#pragma once
#include "GameForm.h"
#include "instrucciones.h"
#include "creditos.h"

namespace Project11 {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class MyMenuform : public System::Windows::Forms::Form
    {
    public:
        MyMenuform(void)
        {
            InitializeComponent();
        }

    protected:
        ~MyMenuform()
        {
            if (components) delete components;
        }

    private:

        // === DECLARACIÓN DE BOTONES (REQUERIDO PARA QUE EL DESIGNER FUNCIONE) ===
        System::Windows::Forms::Button^ btnJugar;
        System::Windows::Forms::Button^ btnInstrucciones;
        System::Windows::Forms::Button^ btnCreditos;
        System::Windows::Forms::Button^ btnSalir;

        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyMenuform::typeid));
            this->btnJugar = (gcnew System::Windows::Forms::Button());
            this->btnInstrucciones = (gcnew System::Windows::Forms::Button());
            this->btnCreditos = (gcnew System::Windows::Forms::Button());
            this->btnSalir = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // btnJugar
            // 
            this->btnJugar->Font = (gcnew System::Drawing::Font(L"Consolas", 26, System::Drawing::FontStyle::Bold));
            this->btnJugar->Location = System::Drawing::Point(528, 594);
            this->btnJugar->Name = L"btnJugar";
            this->btnJugar->Size = System::Drawing::Size(123, 40);
            this->btnJugar->TabIndex = 0;
            this->btnJugar->Text = L"JUGAR";
            this->btnJugar->UseVisualStyleBackColor = true;
            this->btnJugar->Click += gcnew System::EventHandler(this, &MyMenuform::JugarClick);
            // 
            // btnInstrucciones
            // 
            this->btnInstrucciones->Font = (gcnew System::Drawing::Font(L"Consolas", 26, System::Drawing::FontStyle::Bold));
            this->btnInstrucciones->Location = System::Drawing::Point(528, 638);
            this->btnInstrucciones->Name = L"btnInstrucciones";
            this->btnInstrucciones->Size = System::Drawing::Size(304, 48);
            this->btnInstrucciones->TabIndex = 1;
            this->btnInstrucciones->Text = L"INSTRUCCIONES";
            this->btnInstrucciones->UseVisualStyleBackColor = true;
            this->btnInstrucciones->Click += gcnew System::EventHandler(this, &MyMenuform::InstruccionesClick);
            // 
            // btnCreditos
            // 
            this->btnCreditos->Font = (gcnew System::Drawing::Font(L"Consolas", 26, System::Drawing::FontStyle::Bold));
            this->btnCreditos->Location = System::Drawing::Point(657, 594);
            this->btnCreditos->Name = L"btnCreditos";
            this->btnCreditos->Size = System::Drawing::Size(175, 40);
            this->btnCreditos->TabIndex = 2;
            this->btnCreditos->Text = L"CRÉDITOS";
            this->btnCreditos->UseVisualStyleBackColor = true;
            this->btnCreditos->Click += gcnew System::EventHandler(this, &MyMenuform::CreditosClick);
            // 
            // btnSalir
            // 
            this->btnSalir->Font = (gcnew System::Drawing::Font(L"Consolas", 26, System::Drawing::FontStyle::Bold));
            this->btnSalir->Location = System::Drawing::Point(528, 692);
            this->btnSalir->Name = L"btnSalir";
            this->btnSalir->Size = System::Drawing::Size(304, 46);
            this->btnSalir->TabIndex = 3;
            this->btnSalir->Text = L"SALIR";
            this->btnSalir->UseVisualStyleBackColor = true;
            this->btnSalir->Click += gcnew System::EventHandler(this, &MyMenuform::SalirClick);
            // 
            // MyMenuform
            // 
            this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
            this->ClientSize = System::Drawing::Size(974, 776);
            this->Controls->Add(this->btnJugar);
            this->Controls->Add(this->btnInstrucciones);
            this->Controls->Add(this->btnCreditos);
            this->Controls->Add(this->btnSalir);
            this->Name = L"MyMenuform";
            this->Text = L"Menú Principal";
            this->ResumeLayout(false);

        }
#pragma endregion

        // ======================================
        //           EVENTOS
        // ======================================

        void JugarClick(Object^ sender, EventArgs^ e)
        {
            ProjectGame::GameForm^ game = gcnew ProjectGame::GameForm();
            game->Show();
            this->Hide();
        }

        void InstruccionesClick(Object^ sender, EventArgs^ e)
        {
            instrucciones^ inst = gcnew instrucciones();
            inst->ShowDialog();
        }

        void CreditosClick(Object^ sender, EventArgs^ e)
        {
            creditos^ cred = gcnew creditos();
            cred->ShowDialog();
        }

        void SalirClick(Object^ sender, EventArgs^ e)
        {
            Application::Exit();
        }
    };
}
