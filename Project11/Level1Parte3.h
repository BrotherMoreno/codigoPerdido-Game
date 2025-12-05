#pragma once
#include "Capitan.h"
#include "Mushroom.h"
#include "bossMonster.h"
#include "Level1parte2.h"
#include "Nivel2Form.h"

namespace Project11 {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class Level1Parte3 : public Form
    {
    private:

        Capitan^ capitan;
        array<Mushroom^>^ hongos;

        MiniBoss^ miniboss;
        bool minibossSpawned;
        bool minibossEntering;
        bool minibossDead;
        int entradaTargetX;

        Label^ bossDialogLabel;
        array<String^>^ dialogoBoss;
        array<String^>^ dialogoFinalBoss;
        int bossDialogIndex;
        int finalDialogIndex;
        bool bossDialogActive;
        bool showBossFinalDialog;
        int dialogTimer;

        bool waitingForReturn;

        Timer^ gameTimer;
        Bitmap^ fondo;

        Label^ debugLabel;

        int frameCount;
        int fps;
        unsigned long lastFPSTime;

        // =============== D O U B L E   B U F F E R ======================
        BufferedGraphicsContext^ context;
        BufferedGraphics^ buffer;

        // =============== FADE ========================
        int fadeAlpha;
        bool fadingDeath;
        bool fadingToNext;

        Label^ deathLabel;
        Label^ loadingLabel;

        Timer^ fadeTimer;

        System::ComponentModel::Container^ components;

    public:

        Level1Parte3(void)
        {
            InitializeComponent();

            this->DoubleBuffered = false;  // LO DESACTIVAMOS (usaremos buffer real)
            this->KeyPreview = true;
            this->SetStyle(ControlStyles::AllPaintingInWmPaint |
                ControlStyles::UserPaint |
                ControlStyles::OptimizedDoubleBuffer, false);

            // ======= Crear Contexto =======
            context = BufferedGraphicsManager::Current;
            context->MaximumBuffer = Drawing::Size(this->Width + 1, this->Height + 1);
            buffer = context->Allocate(this->CreateGraphics(), this->ClientRectangle);

            // ===== EVENTOS =====
            this->Load += gcnew EventHandler(this, &Level1Parte3::OnLoadForm);
            this->KeyDown += gcnew KeyEventHandler(this, &Level1Parte3::OnKeyDown);
            this->KeyUp += gcnew KeyEventHandler(this, &Level1Parte3::OnKeyUp);

            // ===== DEBUG LABEL =====
            debugLabel = gcnew Label();
            debugLabel->ForeColor = Color::Lime;
            debugLabel->BackColor = Color::FromArgb(120, 0, 0, 0);
            debugLabel->Font = gcnew Drawing::Font("Consolas", 13, FontStyle::Bold);
            debugLabel->AutoSize = true;
            debugLabel->Location = Point(10, 10);
            debugLabel->BringToFront();
            this->Controls->Add(debugLabel);

            // ===== DIÁLOGO JEFE =====
            bossDialogLabel = gcnew Label();
            bossDialogLabel->ForeColor = Color::White;
            bossDialogLabel->BackColor = Color::FromArgb(200, 0, 0, 0);
            bossDialogLabel->Font = gcnew Drawing::Font("Consolas", 20, FontStyle::Bold);
            bossDialogLabel->AutoSize = false;
            bossDialogLabel->Padding = System::Windows::Forms::Padding(20);
            bossDialogLabel->Size = Drawing::Size(1000, 150);
            bossDialogLabel->Location = Point(70, this->ClientSize.Height - 240);
            bossDialogLabel->Visible = false;
            bossDialogLabel->BringToFront();
            this->Controls->Add(bossDialogLabel);

            // ===== FADE TEXTOS =====
            deathLabel = gcnew Label();
            deathLabel->Text = "";
            deathLabel->ForeColor = Color::White;
            deathLabel->BackColor = Color::Transparent;
            deathLabel->Font = gcnew Drawing::Font("Consolas", 40, FontStyle::Bold);
            deathLabel->AutoSize = true;
            deathLabel->Visible = false;
            deathLabel->BringToFront();
            this->Controls->Add(deathLabel);

            loadingLabel = gcnew Label();
            loadingLabel->Text = "";
            loadingLabel->ForeColor = Color::White;
            loadingLabel->BackColor = Color::Transparent;
            loadingLabel->Font = gcnew Drawing::Font("Consolas", 32, FontStyle::Bold);
            loadingLabel->AutoSize = true;
            loadingLabel->Visible = false;
            loadingLabel->BringToFront();
            this->Controls->Add(loadingLabel);

            // ===== FADE TIMER =====
            fadeTimer = gcnew Timer();
            fadeTimer->Interval = 20;
            fadeTimer->Tick += gcnew EventHandler(this, &Level1Parte3::FadeStep);

            fadeAlpha = 0;
            fadingDeath = false;
            fadingToNext = false;

            // ===== GAME TIMER =====
            gameTimer = gcnew Timer();
            gameTimer->Interval = 16;
            gameTimer->Tick += gcnew EventHandler(this, &Level1Parte3::GameLoop);
        }

    protected:
        ~Level1Parte3()
        {
            if (components) delete components;
            if (fondo) delete fondo;
            if (buffer) delete buffer;
        }

#pragma region Designer
    private:
        void InitializeComponent(void)
        {
            System::ComponentModel::ComponentResourceManager^ resources =
                (gcnew System::ComponentModel::ComponentResourceManager(Level1Parte3::typeid));
            this->SuspendLayout();
            this->BackgroundImage =
                (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
            this->ClientSize = System::Drawing::Size(1168, 871);
            this->Name = L"Level1Parte3";
            this->Text = L"Level1Parte3";
            this->ResumeLayout(false);
        }
#pragma endregion

    private:

        void OnLoadForm(Object^ sender, EventArgs^ e)
        {
            capitan = gcnew Capitan(300, 300);

            hongos = gcnew array<Mushroom^>(5);
            Random^ rng = gcnew Random();

            for (int i = 0; i < 5; i++)
            {
                hongos[i] = gcnew Mushroom(
                    rng->Next(100, this->ClientSize.Width - 200),
                    rng->Next(200, this->ClientSize.Height - 200)
                );
                hongos[i]->SetEstado(EstadoHongo::Idle);
            }

            minibossSpawned = false;
            minibossEntering = false;
            minibossDead = false;

            dialogoBoss = gcnew array<String^>{
                "MiniBoss: ...Has logrado llegar hasta aquí.",
                    "MiniBoss: ME VENGARÉ DE NEMESIS 0-11...",
                    "MiniBoss: ¡NINGÚN HUMANO PUEDE DETENERME!"
            };

            dialogoFinalBoss = gcnew array<String^>{
                "MiniBoss: ...Capitán...",
                    "MiniBoss: Nemesis... nos mintió...",
                    "MiniBoss: Dijo que los humanos... nos odiaban...",
                    "MiniBoss: Acusó a los Monarcas... de destruir el Código Maestro...",
                    "MiniBoss: Por eso... busqué venganza...",
                    "MiniBoss: Pero ahora sé... que Nemesis actúa por mente propia...",
                    "MiniBoss: ...Ustedes no crearon el caos...",
                    "MiniBoss: ...Fue él.",
                    "MiniBoss: Capitán... toma esto...",
                    "MiniBoss: Es un poder perdido... te servirá más adelante...",
                    "MiniBoss: ...Adiós..."
            };

            bossDialogIndex = 0;
            finalDialogIndex = 0;
            bossDialogActive = false;
            showBossFinalDialog = false;
            dialogTimer = 0;

            lastFPSTime = Environment::TickCount;

            fadeAlpha = 0;
            fadingDeath = false;
            fadingToNext = false;

            gameTimer->Start();
        }

        // ===========================================================
        //  GAME LOOP
        // ===========================================================
        void GameLoop(Object^ sender, EventArgs^ e)
        {
            capitan->Update();

            // VIVOS HONGOS
            int vivos = 0;
            for each (Mushroom ^ m in hongos)
                if (!m->EstaMuerto()) vivos++;

            // DEBUG
            debugLabel->Text =
                "FPS: " + fps +
                "\nVida: " + capitan->GetVida() +
                "\nBalas: " + capitan->GetBalas() +
                "\nHongos vivos: " + vivos;

            if (showBossFinalDialog)
            {
                dialogTimer++;
                if (dialogTimer > 130)
                {
                    dialogTimer = 0;
                    finalDialogIndex++;

                    if (finalDialogIndex < dialogoFinalBoss->Length)
                    {
                        bossDialogLabel->Text = dialogoFinalBoss[finalDialogIndex];
                    }
                    else
                    {
                        bossDialogLabel->Visible = false;
                        showBossFinalDialog = false;

                        // === FADE A NIVEL 2 ===
                        fadeAlpha = 0;
                        fadingToNext = true;
                        loadingLabel->Text = "CARGANDO...";
                        loadingLabel->Visible = true;
                        CenterLabel(loadingLabel);
                        fadeTimer->Start();
                        gameTimer->Stop();
                        return;
                    }
                }
                Render();
                return;
            }

            // SPAWN MINIBOSS
            if (!minibossSpawned && vivos == 0)
            {
                minibossSpawned = true;
                minibossEntering = true;

                miniboss = gcnew MiniBoss(this->ClientSize.Width + 300, 300);
                miniboss->SetEstado(EstadoMiniBoss::Run);

                entradaTargetX = this->ClientSize.Width / 2 - 200;

                bossDialogActive = true;
                bossDialogLabel->Visible = true;
                bossDialogLabel->Text = dialogoBoss[0];

                Render();
                return;
            }

            // ENTRADA MINIBOSS
            if (minibossEntering)
            {
                miniboss->facingLeft = true;
                miniboss->setX(miniboss->getX() - 6);

                if (miniboss->getX() <= entradaTargetX)
                {
                    minibossEntering = false;
                    dialogTimer = 0;
                    bossDialogIndex = 0;
                }

                miniboss->Update();
                Render();
                return;
            }

            // DIÁLOGO BOSS
            if (bossDialogActive)
            {
                dialogTimer++;

                if (dialogTimer > 120)
                {
                    dialogTimer = 0;
                    bossDialogIndex++;

                    if (bossDialogIndex < dialogoBoss->Length)
                        bossDialogLabel->Text = dialogoBoss[bossDialogIndex];
                    else
                        bossDialogActive = false;
                }

                Render();
                return;
            }

            // IA HONGOS
            UpdateHongos();

            // DISPAROS
            UpdateBalasVsHongos();
            UpdateBalasVsMiniBoss();

            // IA MINIBOSS
            UpdateMiniBoss();

            // MORIR CAPITÁN
            if (capitan->GetVida() <= 0 && !fadingDeath)
            {
                fadingDeath = true;
                fadeAlpha = 0;
                deathLabel->Text = "HAS MUERTO…";
                deathLabel->Visible = true;
                CenterLabel(deathLabel);
                fadeTimer->Start();
                gameTimer->Stop();
                return;
            }

            // FPS
            frameCount++;
            unsigned long now = Environment::TickCount;
            if (now - lastFPSTime >= 1000)
            {
                fps = frameCount;
                frameCount = 0;
                lastFPSTime = now;
            }

            Render();
        }

        // ===========================================================
        //  RENDER USANDO BUFFER REAL (NUNCA FLICKER)
        // ===========================================================
        void Render()
        {
            Graphics^ g = buffer->Graphics;
            g->Clear(Color::Black);

            // Fondo
            if (this->BackgroundImage != nullptr)
                g->DrawImage(this->BackgroundImage, 0, 0, this->ClientSize.Width, this->ClientSize.Height);

            // Hongos
            for each (Mushroom ^ h in hongos)
                if (!h->EstaMuerto())
                    h->Draw(g);

            // Miniboss
            if (minibossSpawned)
                miniboss->Draw(g);

            // Capitán
            capitan->Draw(g);

            // Fade negro
            if (fadingDeath || fadingToNext)
            {
                using namespace System::Drawing;
                g->FillRectangle(gcnew SolidBrush(Color::FromArgb(fadeAlpha, 0, 0, 0)),
                    0, 0, this->ClientSize.Width, this->ClientSize.Height);
            }

            buffer->Render();
        }

        // ===========================================================
        // FADE HANDLER
        // ===========================================================
        void FadeStep(Object^ sender, EventArgs^ e)
        {
            fadeAlpha += 8;
            if (fadeAlpha > 255) fadeAlpha = 255;

            Render();

            if (fadeAlpha >= 255)
            {
                fadeTimer->Stop();

                if (fadingDeath)
                {
                    fadingDeath = false;
                    Level1Parte3^ nuevo = gcnew Level1Parte3();
                    nuevo->Show();
                    this->Hide();
                }
                else if (fadingToNext)
                {
                    fadingToNext = false;

                    Nivel2Form^ lvl2 = gcnew Nivel2Form();
                    lvl2->Show();

                    this->Hide();   // <--- AQUÍ EL CAMBIO CORRECTO
                }
            }
        }

        // ===========================================================
        // UTILS
        // ===========================================================
        void CenterLabel(Label^ label)
        {
            label->Left = (this->ClientSize.Width - label->Width) / 2;
            label->Top = (this->ClientSize.Height - label->Height) / 2;
        }

        void UpdateHongos()
        {
            for each (Mushroom ^ h in hongos)
            {
                if (h->EstaMuerto()) continue;

                int dx = capitan->getX() - h->getX();
                int dy = capitan->getY() - h->getY();
                double dist = Math::Sqrt(dx * dx + dy * dy);

                h->facingLeft = (dx < 0);

                if (dist > 60)
                {
                    h->setX(h->getX() + (dx > 0 ? 2 : -2));
                    h->setY(h->getY() + (dy > 0 ? 2 : -2));
                    h->SetEstado(EstadoHongo::Run);
                }
                else
                {
                    h->SetEstado(EstadoHongo::Attack);
                    if (h->getHitbox().IntersectsWith(capitan->getHitbox()))
                        capitan->setVida(capitan->GetVida() - 2);
                }

                h->Update();
            }
        }

        void UpdateBalasVsHongos()
        {
            for (int i = capitan->balas->Count - 1; i >= 0; i--)
            {
                Bala^ b = capitan->balas[i];
                Rectangle hb = b->getHitbox();

                for each (Mushroom ^ h in hongos)
                {
                    if (!h->EstaMuerto() && hb.IntersectsWith(h->getHitbox()))
                    {
                        h->setVida(h->getVida() - 8);
                        h->SetEstado(EstadoHongo::Hit);
                        capitan->balas->RemoveAt(i);
                        return;
                    }
                }
            }
        }

        void UpdateBalasVsMiniBoss()
        {
            if (!minibossSpawned || miniboss == nullptr || minibossDead)
                return;

            for (int i = capitan->balas->Count - 1; i >= 0; i--)
            {
                Bala^ b = capitan->balas[i];
                if (b->getHitbox().IntersectsWith(miniboss->getHitbox()))
                {
                    miniboss->setVida(miniboss->getVida() - 12);
                    miniboss->SetEstado(EstadoMiniBoss::Hit);
                    capitan->balas->RemoveAt(i);
                    return;
                }
            }
        }

        void UpdateMiniBoss()
        {
            if (!minibossSpawned || miniboss == nullptr)
                return;

            if (minibossDead)
            {
                miniboss->Update();

                if (miniboss->IsDeathFinished())
                {
                    showBossFinalDialog = true;
                    finalDialogIndex = 0;
                    bossDialogLabel->Visible = true;
                    bossDialogLabel->Text = dialogoFinalBoss[0];
                }
                return;
            }

            int dx = capitan->getX() - miniboss->getX();
            int dy = capitan->getY() - miniboss->getY();
            double dist = Math::Sqrt(dx * dx + dy * dy);

            miniboss->facingLeft = (dx < 0);

            if (dist > 100)
            {
                // velocidad MÁS LENTA (antes 4, ahora 2)
                miniboss->setX(miniboss->getX() + (dx > 0 ? 2 : -2));
                miniboss->setY(miniboss->getY() + (dy > 0 ? 2 : -2));
                miniboss->SetEstado(EstadoMiniBoss::Run);
            }
            else
            {
                miniboss->SetEstado(EstadoMiniBoss::Attack);
                if (miniboss->getAttackHitbox().IntersectsWith(capitan->getHitbox()))
                    capitan->setVida(capitan->GetVida() - 2); // daño REDUCIDO (antes 5)
            }

            miniboss->Update();

            if (miniboss->getVida() <= 0)
            {
                minibossDead = true;
                miniboss->SetEstado(EstadoMiniBoss::Death);
            }
        }

    private:

        void OnKeyDown(Object^ sender, KeyEventArgs^ e)
        {
            if (!bossDialogActive && !minibossEntering && !showBossFinalDialog)
                capitan->KeyDown(e);
        }

        void OnKeyUp(Object^ sender, KeyEventArgs^ e)
        {
            if (!bossDialogActive && !minibossEntering && !showBossFinalDialog)
                capitan->KeyUp(e);
        }

    };
}
