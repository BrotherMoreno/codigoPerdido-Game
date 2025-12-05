#pragma once
#include "Capitan.h"
#include "Trader.h"
#include "Level1Parte3.h"

namespace Project11 {
    
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class Level1parte2 : public Form
    {
    private:

        Capitan^ capitan;
        Trader^ trader;

        Timer^ gameTimer;
        Timer^ fadeTimer;

        System::ComponentModel::Container^ components;

        Bitmap^ fondo;

        // -------------------------
        //   DIÁLOGO
        // -------------------------
        array<String^>^ dialogoTrader;
        int dialogoIndex;
        Label^ dialogLabel;

        // -------------------------
        //   HUD DEBUG
        // -------------------------
        Label^ debugLabel;
        bool debugVisible;
        bool showHitbox;

        // FPS
        int frameCount;
        int fps;
        unsigned long lastFPSTime;

        // -------------------------
        //   CONTROL DE NIVEL
        // -------------------------
        bool talkedToTrader = false;
        bool isTransitioning = false;
        int fadeAlpha = 0;

        // -------------------------
        //   AVISO DE SALIDA
        // -------------------------
        bool showExitMessage = false;
        int exitMessageTimer = 0;

    private:
        bool IsDesignMode()
        {
            return (System::ComponentModel::LicenseManager::UsageMode ==
                System::ComponentModel::LicenseUsageMode::Designtime);
        }

    public:
        Level1parte2(void)
        {
            InitializeComponent();

            if (IsDesignMode()) return;

            this->DoubleBuffered = true;
            this->KeyPreview = true;

            this->Load += gcnew EventHandler(this, &Level1parte2::OnLoadForm);

            // -----------------------------
            // LABEL DIÁLOGO
            // -----------------------------
            dialogLabel = gcnew Label();
            dialogLabel->ForeColor = Color::White;
            dialogLabel->BackColor = Color::FromArgb(180, 0, 0, 0);
            dialogLabel->Font = gcnew Drawing::Font("Consolas", 18, FontStyle::Bold);
            dialogLabel->AutoSize = false;
            dialogLabel->Size = Drawing::Size(1050, 150);
            dialogLabel->Location = Point(40, 600);
            dialogLabel->Visible = false;
            dialogLabel->Padding = System::Windows::Forms::Padding(10);
            this->Controls->Add(dialogLabel);

            // -----------------------------
            // DIALOGOS
            // -----------------------------
            dialogoTrader = gcnew array<String^>{
                "Trader: Eh, capitan. Esa nave tuya... parece que se va a caer a pedazos.",
                    "Capitan: El modulo de propulsion se destruyo al caer aqui.",
                    "Trader: Puedo ayudarte. Tengo piezas para fabricar un modulo nuevo.",
                    "Capitan: Lo necesito para seguir buscando los fragmentos del codigo maestro.",
                    "Trader: El bosque esta lleno de criaturas interfiriendo mis sensores.",
                    "Trader: Si lo limpias, te dare un modulo de propulsion totalmente funcional.",
                    "Capitan: Trato hecho. Dame las ubicaciones.",
                    "Trader: Sigue el puente hacia el sur y encontraras el lugar que te indique."
            };

            dialogoIndex = 0;

            // -----------------------------
            // HUD DEBUG
            // -----------------------------
            debugLabel = gcnew Label();
            debugLabel->ForeColor = Color::Lime;
            debugLabel->BackColor = Color::FromArgb(120, 0, 0, 0);
            debugLabel->Font = gcnew Drawing::Font("Consolas", 13, FontStyle::Bold);
            debugLabel->AutoSize = true;
            debugLabel->Location = Point(10, 10);
            debugLabel->Visible = true;

            debugVisible = true;
            showHitbox = false;

            this->Controls->Add(debugLabel);

            // -----------------------------
            // FPS INIT
            // -----------------------------
            frameCount = 0;
            fps = 0;
            lastFPSTime = Environment::TickCount;

            // -----------------------------
            // TIMER PRINCIPAL
            // -----------------------------
            gameTimer = gcnew Timer();
            gameTimer->Interval = 16;
            gameTimer->Tick += gcnew EventHandler(this, &Level1parte2::GameLoop);
            gameTimer->Start();

            // KEY EVENTS
            this->KeyDown += gcnew KeyEventHandler(this, &Level1parte2::OnKeyDown);
            this->KeyUp += gcnew KeyEventHandler(this, &Level1parte2::OnKeyUp);

            // -----------------------------
            // TIMER FADE
            // -----------------------------
            fadeTimer = gcnew Timer();
            fadeTimer->Interval = 20;
            fadeTimer->Tick += gcnew EventHandler(this, &Level1parte2::FadeStep);
        }

    protected:
        ~Level1parte2()
        {
            if (components) delete components;
            if (fondo) delete fondo;
        }

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->SuspendLayout();
            this->ClientSize = System::Drawing::Size(1142, 777);
            this->Name = L"Level1parte2";
            this->Text = L"Level1parte2";
            this->ResumeLayout(false);
        }
#pragma endregion

    private:

        // =======================================================
        // LOAD
        // =======================================================
        void OnLoadForm(Object^ sender, EventArgs^ e)
        {
            if (IsDesignMode()) return;

            Bitmap^ fondoOriginal = gcnew Bitmap("fondolevel1.png");

            fondo = gcnew Bitmap(this->ClientSize.Width, this->ClientSize.Height);
            Graphics^ gf = Graphics::FromImage(fondo);
            gf->DrawImage(fondoOriginal, Rectangle(0, 0, this->ClientSize.Width, this->ClientSize.Height));
            delete gf;
            delete fondoOriginal;

            capitan = gcnew Capitan(312, 300);
            trader = gcnew Trader(552, 600);
        }

        // =======================================================
        // GAME LOOP
        // =======================================================
        void GameLoop(Object^ sender, EventArgs^ e)
        {
            if (capitan == nullptr || trader == nullptr) return;

            capitan->Update();
            trader->Update(capitan->getHitbox());

            // FPS
            frameCount++;
            unsigned long now = Environment::TickCount;
            if (now - lastFPSTime >= 1000)
            {
                fps = frameCount;
                frameCount = 0;
                lastFPSTime = now;
            }

            // HUD
            if (debugVisible)
            {
                debugLabel->Text =
                    "X: " + capitan->getX() +
                    "   Y: " + capitan->getY() +
                    "\nFPS: " + fps;
            }

            int cx = capitan->getX();
            int cy = capitan->getY();

            // =====================================================
            // AVISO SI QUIERE SALIR SIN HABLAR
            // =====================================================
            if (!talkedToTrader &&
                cx >= 468 && cx <= 568 &&
                cy >= 724)
            {
                showExitMessage = true;
                exitMessageTimer = 60; // ~1 segundo visible
            }

            if (exitMessageTimer > 0)
            {
                exitMessageTimer--;
                if (exitMessageTimer == 0)
                    showExitMessage = false;
            }

            // =====================================================
            // CAMBIO DE NIVEL (SI YA HABLÓ)
            // =====================================================
            if (!isTransitioning &&
                talkedToTrader &&
                cx >= 468 && cx <= 568 &&
                cy >= 724)
            {
                isTransitioning = true;
                fadeAlpha = 0;
                fadeTimer->Start();
            }

            this->Invalidate();
        }

        // =======================================================
        // RENDER
        // =======================================================
    protected:
        virtual void OnPaint(PaintEventArgs^ e) override
        {
            if (capitan == nullptr || trader == nullptr) return;

            Graphics^ g = e->Graphics;
            g->Clear(Color::Black);

            if (fondo != nullptr)
                g->DrawImage(fondo, 0, 0, this->ClientSize.Width, this->ClientSize.Height);

            capitan->Draw(g);
            trader->Draw(g);

            // INDICADOR DE HABLAR
            int dist = Math::Abs(capitan->getHitbox().X - trader->getHitbox().X)
                + Math::Abs(capitan->getHitbox().Y - trader->getHitbox().Y);

            if (dist < 200 && !dialogLabel->Visible)
            {
                g->DrawString(
                    "TRADER [Q] → Hablar",
                    gcnew Drawing::Font("Consolas", 12, FontStyle::Bold),
                    Brushes::White,
                    trader->getHitbox().X + 10,
                    trader->getHitbox().Y - 30
                );
            }

            // -------------------------
            // AVISO DE SALIDA
            // -------------------------
            if (showExitMessage)
            {
                g->DrawString(
                    "Debes hablar con el Trader antes de continuar.",
                    gcnew Drawing::Font("Consolas", 16, FontStyle::Bold),
                    Brushes::Yellow,
                    capitan->getX() - 50,
                    capitan->getY() - 40
                );
            }

            // HITBOX DEBUG
            if (showHitbox)
            {
                Pen^ p = gcnew Pen(Color::Red, 2);
                g->DrawRectangle(p, capitan->getHitbox());
                g->DrawRectangle(p, trader->getHitbox());
                delete p;
            }

            // FADE OUT
            if (isTransitioning)
            {
                SolidBrush^ sb = gcnew SolidBrush(Color::FromArgb(fadeAlpha, 0, 0, 0));
                g->FillRectangle(sb, 0, 0, this->ClientSize.Width, this->ClientSize.Height);
                delete sb;
            }
        }

        // =======================================================
        // FADE
        // =======================================================
    private:
        void FadeStep(Object^ sender, EventArgs^ e)
        {
            fadeAlpha += 10;

            if (fadeAlpha >= 255)
            {
                fadeTimer->Stop();
                fadeAlpha = 255;

                Level1Parte3^ next = gcnew Level1Parte3();
                next->Show();
                this->Hide();
            }

            this->Invalidate();
        }

        // =======================================================
        // INPUT
        // =======================================================
        void OnKeyDown(Object^ sender, KeyEventArgs^ e)
        {
            if (capitan != nullptr)
                capitan->KeyDown(e);

            // HABLAR CON Q
            if (e->KeyCode == Keys::Q &&
                trader->GetEstado() == EstadoTrader::Dialogue)
            {
                if (!dialogLabel->Visible)
                {
                    dialogLabel->Visible = true;
                    dialogoIndex = 0;
                    dialogLabel->Text = dialogoTrader[dialogoIndex];
                    talkedToTrader = true;
                }
                else
                {
                    dialogoIndex++;

                    if (dialogoIndex < dialogoTrader->Length)
                        dialogLabel->Text = dialogoTrader[dialogoIndex];
                    else
                        dialogLabel->Visible = false;
                }
            }

            if (e->KeyCode == Keys::F3)
            {
                debugVisible = !debugVisible;
                debugLabel->Visible = debugVisible;
            }

            if (e->KeyCode == Keys::F4)
            {
                showHitbox = !showHitbox;
            }
        }

        void OnKeyUp(Object^ sender, KeyEventArgs^ e)
        {
            if (capitan != nullptr)
                capitan->KeyUp(e);
        }
    };
}

