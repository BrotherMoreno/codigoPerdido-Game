#include "GameForm.h"
#include "Level1parte2.h"
#include "Level1parte3.h"
#include "Nivel2Form.h"
#include "MyMenuform.h"
using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Cambia "ProjectGame" por el namespace de tu proyecto si es distinto
    Project11::MyMenuform form;

    Application::Run(% form);
    return 0;
}
