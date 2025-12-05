#pragma once
#include "Parallax.h"
#include "Nave.h"
#include "Level1parte2.h"   // <-- AHORA SÍ EL FORM REAL

namespace ProjectGame
{
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Media;

    public ref class GameForm : public Form
    {
    private:
        Parallax^ parallax;
        Nave^ nave;
        Timer^ timer1;

        // --- SISTEMA DE MAQUINA DE ESCRIBIR ---
        array<String^>^ dialogosTexto;
        array<String^>^ dialogosAudio;
        int dialogIndex;
        int charIndex;

        Label^ typeLabel;
        Timer^ typeTimer;
        SoundPlayer^ sfx;

        // --- TIMER PARA ESPERAR 4 SEGUNDOS ---
        Timer^ endMissionTimer;

    public:
        GameForm(void)
        {
            InitializeComponent();

            this->DoubleBuffered = true;

            parallax = gcnew Parallax(this->Width, this->Height);
            nave = gcnew Nave(200, 300);

            timer1 = gcnew Timer();
            timer1->Interval = 16;
            timer1->Tick += gcnew EventHandler(this, &GameForm::GameLoop);
            timer1->Start();

            sfx = gcnew SoundPlayer();

            // ========================
            // DIALOGOS + AUDIOS
            // ========================
            dialogosTexto = gcnew array<String^>{
                "Capitan... ¿me copia? El codigo maestro... el que activa toda la red de defensa... desaparecio del sistema. No sabemos como pudo perderse...",
                    "Control... lo escucho. ¿Esta diciendo que el unico codigo que puede salvar al planeta... ya no esta? Necesito todos los detalles, ahora.",

                    "Los registros indican que el codigo fue fragmentado. Dividido en varias piezas y dispersado por el sector. Esto no deberia ser posible...",
                    "Entonces fue sabotaje. Control, digame si tenemos alguna señal de alguno de esos fragmentos. No puedo buscarlos a ciegas.",

                    "Estoy tratando de rastrear la ultima ubicacion, pero todos los sensores estan cayendo. No responden a ningun comando... algo esta derribando el sistema desde dentro.",
                    "Control... mantengase enfocado. Si el sistema cae, necesito una ultima lectura antes del apagon total. Solo una.",

                    "Capitan... necesito un reporte inmediato de su posicion. La comunicacion es inestable y estamos perdiendo visibilidad desde aqui.",
                    "Sector 7-B... avanzando hacia el modulo exterior. Pero si seguimos perdiendo señal, no podre recibir mas instrucciones. Confirme si tiene algo mas.",

                    "Si no recuperamos ese codigo... la red no podra activarse. El planeta quedara completamente indefenso. Por favor... tiene que encontrar los fragmentos.",
                    "No se preocupen por mi, Control. Preocupense por el mundo. Hare lo que sea necesario para recuperar ese codigo. Pero necesito que aguanten un poco mas.",

                    "No creo que sea un fallo tecnico... alguien entro al sistema. Cortaron secciones del centro de control. Estoy solo aqui... no se que esta pasando.",
                    "¿Solo? Eso significa que el saboteador sigue cerca. Bloquee accesos si aun puede. No permita que destruyan lo que queda del sistema.",

                    "Antes de que todo se apagara... el fragmento principal estaba cerca del limite del modulo exterior. Es lo ultimo que registramos.",
                    "Recibido. Me dirigire alli de inmediato. Si no vuelvo a escuchar su voz... avanzare igual. No dejare que el mundo caiga por esto.",

                    "Capitan... la comunicacion... se esta... ca... yen... do... Si deja de oirme... usted sera la unica unidad activa. El codigo... tiene que... encontrarlo... antes de que... todo se pierd...",
                    "Control... Control... la señal se esta yendo. Si perdemos el enlace... estare completamente solo. Pero no voy a dejar que ese codigo desaparezca... Control... ¿me escuchan?... Control...",

                    "INICIO DE MISIÓN:\nRecupera los fragmentos del codigo maestro dispersos por el sector.\nCada pieza es vital para reactivar la red de defensa global.\nEvita interferencia enemiga y mantén la nave estable.\nTu misión comienza ahora, Capitán…"
            };

            dialogosAudio = gcnew array<String^>{
                "audios/primerdialogoCC.wav",
                    "audios/dialogocapitan1.wav",

                    "audios/segundodialogoCC.wav",
                    "audios/dialogocapitan2.wav",

                    "audios/tercerdialogoCC.wav",
                    "audios/dialogocapitan3.wav",

                    "audios/cuartodialogoCC.wav",
                    "audios/dialogocapitan4.wav",

                    "audios/quintodialogoCC.wav",
                    "audios/dialogocapitan5.wav",

                    "audios/sextodialogoCC.wav",
                    "audios/dialogocapitan6.wav",

                    "audios/septimodialogoCC.wav",
                    "audios/dialogocapitan7.wav",

                    "audios/octavodialogoCC.wav",
                    "audios/dialogocapitan8.wav",

                    ""
            };

            dialogIndex = 0;
            charIndex = 0;

            // LABEL DEL TEXTO
            typeLabel = gcnew Label();
            typeLabel->ForeColor = Color::White;
            typeLabel->BackColor = Color::FromArgb(160, 0, 0, 0);
            typeLabel->Font = gcnew Drawing::Font("Consolas", 20, FontStyle::Bold);
            typeLabel->AutoSize = false;
            typeLabel->Size = System::Drawing::Size(1220, 200);
            typeLabel->Location = Point(30, 500);
            this->Controls->Add(typeLabel);

            // TIMER DEL TYPEWRITER
            typeTimer = gcnew Timer();
            typeTimer->Interval = 35;
            typeTimer->Tick += gcnew EventHandler(this, &GameForm::Typewriter);
            typeTimer->Start();

            // TIMER QUE A LOS 4s CAMBIA DE FORM
            endMissionTimer = gcnew Timer();
            endMissionTimer->Interval = 4000;
            endMissionTimer->Tick += gcnew EventHandler(this, &GameForm::GoToNextForm);

            this->KeyDown += gcnew KeyEventHandler(this, &GameForm::NextDialog);
        }

    protected:
        ~GameForm()
        {
            if (components) delete components;
        }

    private:
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->SuspendLayout();
            this->ClientSize = System::Drawing::Size(1280, 720);
            this->Name = L"GameForm";
            this->Text = L"Game Window";
            this->ResumeLayout(false);
        }
#pragma endregion

        void GameLoop(Object^ sender, EventArgs^ e)
        {
            parallax->Update();
            nave->Update();
            this->Invalidate();
        }

    protected:
        virtual void OnPaint(PaintEventArgs^ e) override
        {
            Form::OnPaint(e);
            parallax->Draw(e->Graphics);
            nave->Draw(e->Graphics);
        }

        void StartDialogAudio()
        {
            if (dialogIndex >= dialogosAudio->Length)
                return;

            if (dialogosAudio[dialogIndex] == "")
                return;

            try
            {
                sfx->SoundLocation = dialogosAudio[dialogIndex];
                sfx->Load();
                sfx->Play();
            }
            catch (...) {}
        }

        void Typewriter(Object^ sender, EventArgs^ e)
        {
            if (dialogIndex >= dialogosTexto->Length)
            {
                typeTimer->Stop();
                return;
            }

            if (charIndex == 0)
                StartDialogAudio();

            String^ texto = dialogosTexto[dialogIndex];

            if (charIndex < texto->Length)
            {
                typeLabel->Text += texto[charIndex];
                charIndex++;
            }
            else
            {
                typeTimer->Stop();
            }
        }

        void NextDialog(Object^ sender, KeyEventArgs^ e)
        {
            if (e->KeyCode != Keys::Space)
                return;

            if (dialogIndex >= dialogosTexto->Length)
                return;

            if (charIndex < dialogosTexto[dialogIndex]->Length)
            {
                typeLabel->Text = dialogosTexto[dialogIndex];
                charIndex = dialogosTexto[dialogIndex]->Length;
                return;
            }

            dialogIndex++;

            if (dialogIndex < dialogosTexto->Length)
            {
                typeLabel->Text = "";
                charIndex = 0;
                typeTimer->Start();
            }
            else
            {
                typeLabel->Visible = false;
                endMissionTimer->Start();
            }
        }

        // ================================
        //     CAMBIAR AL OTRO FORM
        // ================================
        void GoToNextForm(Object^ sender, EventArgs^ e)
        {
            endMissionTimer->Stop();

            // ABRIR Level1parte2 con SU PROPIO TAMAÑO
             Project11::Level1parte2^ lvl = gcnew Project11::Level1parte2();

            lvl->Show();
            this->Hide();
        }
    };
}
