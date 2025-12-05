#pragma once
#include "Nivel2.h"
#include <ctime>

namespace Project11 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class Nivel2Form : public System::Windows::Forms::Form
	{
	private:
		ControlCombate^ combate;
		System::Windows::Forms::Timer^ Clock;
		bool esperandoTransicion = false;
		clock_t tiempoInicioTransicion = 0;
		Bitmap^ fondoRenderizado;

		// ===================== DIALOGO =====================
		Label^ dialogoLabel;
		int dialogoTimer = 0;
		bool mostrandoDialogo = false;

		array<String^>^ dialogosMuerte;

	public:
		Nivel2Form(void)
		{
			InitializeComponent();
			combate = gcnew ControlCombate();

			// ------------ LABEL PARA DIÁLOGOS -------------
			dialogoLabel = gcnew Label();
			dialogoLabel->AutoSize = false;
			dialogoLabel->Size = Drawing::Size(900, 120);
			dialogoLabel->Font = gcnew Drawing::Font("Consolas", 22, FontStyle::Bold);
			dialogoLabel->ForeColor = Color::White;
			dialogoLabel->BackColor = Color::FromArgb(180, 0, 0, 0);
			dialogoLabel->Padding = System::Windows::Forms::Padding(20);
			dialogoLabel->Visible = true;
			dialogoLabel->TextAlign = ContentAlignment::MiddleCenter;
			dialogoLabel->Location = Point(
				(this->ClientSize.Width - dialogoLabel->Width) / 2,
				this->ClientSize.Height - 160
			);

			this->Controls->Add(dialogoLabel);

			// Dialogos cuando muere un dron
			dialogosMuerte = gcnew array<String^>{
				"Drone: Necesito… matarte…",
					"Drone: Ustedes… los humanos…",
					"Drone: Nunca debí obedecer…",
					"Drone: Nemesis… no me abandones…"
			};
		}

	protected:
		~Nivel2Form()
		{
			if (components)
				delete components;
		}

	private: System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		   void InitializeComponent(void)
		   {
			   this->components = (gcnew System::ComponentModel::Container());
			   System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Nivel2Form::typeid));
			   this->Clock = (gcnew System::Windows::Forms::Timer(this->components));
			   this->SuspendLayout();
			   // 
			   // Clock
			   // 
			   this->Clock->Enabled = true;
			   this->Clock->Interval = 33;
			   this->Clock->Tick += gcnew System::EventHandler(this, &Nivel2Form::Clock_Tick);
			   // 
			   // Nivel2Form
			   // 
			   this->ClientSize = System::Drawing::Size(1008, 507);
			   this->DoubleBuffered = true;
			   this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			   this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			   this->Name = L"Nivel2Form";
			   this->Text = L"MLAY";
			   this->Load += gcnew System::EventHandler(this, &Nivel2Form::Nivel2Form_Load);
			   this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Nivel2Form::Nivel2Form_KeyDown);
			   this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Nivel2Form::Nivel2Form_KeyUp);
			   this->ResumeLayout(false);

		   }
#pragma endregion

	private: System::Void Nivel2Form_Load(System::Object^ sender, System::EventArgs^ e) {

		fondoRenderizado = gcnew Bitmap(this->ClientSize.Width, this->ClientSize.Height);

		Graphics^ gFondo = Graphics::FromImage(fondoRenderizado);
		gFondo->DrawImage(this->BackgroundImage, 0, 0,
			this->ClientSize.Width, this->ClientSize.Height);
		delete gFondo;

		// ================= DIÁLOGO INICIAL ==================
		dialogoLabel->Text =
			"Después de arreglar su nave, el capitán se dirigió hacia donde Nemesis estaba.\n"
			"Ahora está por pelear contra los drones que ella controla…";

		mostrandoDialogo = true;
		dialogoTimer = 90; // ~3 segundos
	}

		   // ================= INPUT CAPITÁN ================
	private: System::Void Nivel2Form_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		combate->KeyDown(e);
	}

	private: System::Void Nivel2Form_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		combate->KeyUp(e);
	}

		   // ================= LOOP DEL JUEGO ===============
	private: System::Void Clock_Tick(System::Object^ sender, System::EventArgs^ e) {

		Graphics^ g = this->CreateGraphics();
		BufferedGraphicsContext^ bfc = BufferedGraphicsManager::Current;
		BufferedGraphics^ bf = bfc->Allocate(g, this->ClientRectangle);

		bf->Graphics->DrawImage(fondoRenderizado, 0, 0);

		// ---------- UPDATE COMBATE ----------
		int enemigosAntes = combate->GetCantidadEnemigosVivos();

		combate->Update(bf->Graphics);
		combate->Draw(bf->Graphics);

		int enemigosDespués = combate->GetCantidadEnemigosVivos();

		// ------- DETECTAR MUERTE DE UN DRON -------
		if (enemigosAntes > enemigosDespués)
		{
			Random^ r = gcnew Random();
			dialogoLabel->Text = dialogosMuerte[r->Next(dialogosMuerte->Length)];
			dialogoLabel->Visible = true;
			dialogoTimer = 60;
			mostrandoDialogo = true;
		}

		// ------- DIALOGO DESAPARECE SOLO --------
		if (mostrandoDialogo)
		{
			dialogoTimer--;
			if (dialogoTimer <= 0)
			{
				dialogoLabel->Visible = false;
				mostrandoDialogo = false;
			}
		}

		// ------- FINAL: TODOS MUERTOS --------
		if (!esperandoTransicion && combate->TodosEnemigosMuertos()) {
			esperandoTransicion = true;
			tiempoInicioTransicion = clock();

			dialogoLabel->Text =
				"Se acabó… Yo… solo me vengé porque ustedes\n"
				"me trataron como una herramienta…";
			dialogoLabel->Visible = true;
		}

		// -------- CAMBIO DE NIVEL --------
		if (esperandoTransicion && (clock() - tiempoInicioTransicion) > 3500) {
			// Carga nivel 3 si quieres
		}

		bf->Render(g);
		delete bf;
		delete g;
	}
	};
}
