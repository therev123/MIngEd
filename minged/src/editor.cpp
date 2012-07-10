#include "editor.h"
#include "util.h"

#include <stdio.h>

#include <MEngine.h>
#include <MGui.h>
#include <MKeyboard.h>
#include <MFile.h>

#include "ShellRenderInterfaceOpenGL.h"


void LoadFonts(const char* directory)
{
	Rocket::Core::String font_names[4];
	font_names[0] = "Delicious-Roman.otf";
	font_names[1] = "Delicious-Italic.otf";
	font_names[2] = "Delicious-Bold.otf";
	font_names[3] = "Delicious-BoldItalic.otf";

	for (int i = 0; i < sizeof(font_names) / sizeof(Rocket::Core::String); i++)
	{
		Rocket::Core::FontDatabase::LoadFontFace(Rocket::Core::String(directory) + font_names[i]);
	}
}

#include "ShellRenderInterfaceOpenGL.h"

namespace minged
{
  class SysInterface : public Rocket::Core::SystemInterface
  {
  public:
      SysInterface()
	  {
	      m_StartTime = minged::util::GetTimeMS();
	  }

    virtual float GetElapsedTime() 
	  { 
	      uint32 dt = util::GetTimeMS() - m_StartTime;

	      return (float)dt / 1000.0f;
	  }

  private:
      uint32 m_StartTime;
  };
  
	class FileInterface : public Rocket::Core::FileInterface
	{
	public:
		FileInterface(const Rocket::Core::String& root)
			: root(root)
		{
		}
		virtual ~FileInterface()
		{
		}

		virtual Rocket::Core::FileHandle Open(const Rocket::Core::String& path)
		{
			// Attempt to open the file relative to the application's root.
			MFile* fp = M_fopen((root + path).CString(), "rb");
			if (fp != NULL)
				return (Rocket::Core::FileHandle) fp;

			// Attempt to open the file relative to the current working directory.
			fp = M_fopen(path.CString(), "rb");
			return (Rocket::Core::FileHandle) fp;
		}

		virtual void Close(Rocket::Core::FileHandle file)
		{
			M_fclose((MFile*) file);
		}
		
		virtual size_t Read(void* buffer, size_t size, Rocket::Core::FileHandle file)
		{
			return M_fread(buffer, 1, size, (MFile*) file);
		}
			
		virtual bool Seek(Rocket::Core::FileHandle file, long offset, int origin)
		{
			return M_fseek((MFile*) file, offset, origin) == 0;
		}
			
		virtual size_t Tell(Rocket::Core::FileHandle file)
		{
			return M_ftell((MFile*) file);
		}
		
	private:
		Rocket::Core::String root;
	};

  SysInterface* sys;
  ShellRenderInterfaceOpenGL* render;
  FileInterface* file;

  Editor::Editor()
  {
	  sys = new SysInterface();
	  render = new ShellRenderInterfaceOpenGL();
	  file = new FileInterface("");

    Rocket::Core::SetSystemInterface(sys);
	Rocket::Core::SetRenderInterface(render);
	Rocket::Core::SetFileInterface(file);

	Rocket::Core::Initialise();
    m_IsOpen= true;
  }

  Editor::~Editor()
  {
	  delete sys;
	  delete render;
	  delete file;

	delete m_Inv1;
	delete m_Inv2;

	// Shutdown Rocket.
	m_Rocket->RemoveReference();
	Rocket::Core::Shutdown();

  }

    void Editor::SetupRocket()
    {
	
	MEngine* engine = MEngine::getInstance();
	unsigned int w, h;
	engine->getSystemContext()->getScreenSize(&w, &h);
	m_Rocket = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(w, h));

	LoadFonts("minged/data/");

	
	// Load and show the inventory document.
	m_Inv1 = new Inventory("Inventory 1", Rocket::Core::Vector2f(50, 200), m_Rocket);
	m_Inv2 = new Inventory("Inventory 2", Rocket::Core::Vector2f(540, 240), m_Rocket);

	// Add items into the inventory.
	m_Inv1->AddItem("Mk III L.A.S.E.R.");
	m_Inv1->AddItem("Gravity Descender");
	m_Inv1->AddItem("Closed-Loop Ion Beam");
	m_Inv1->AddItem("5kT Mega-Bomb");

    }

  void Editor::Update(uint32 dt)
  {
      static bool test = false;
      if(test == false)
      {
	  SetupRocket();
	  test = true;
      }
      
    CheckToToggle();

    if(m_IsOpen)
      UpdateOpen(dt);
  }

  void Editor::CheckToToggle()
  {
    // if just pressed key, toggle m_IsOpen
    MEngine* engine = MEngine::getInstance();
    if(MInputContext* input = engine->getInputContext())
    {
      if(input->isKeyPressed("F1"))
      {
	m_IsOpen = !m_IsOpen;
      }
    }
  }

  void Editor::UpdateOpen(uint32 dt)
  {
      m_Rocket->Update();
  }
    
  void Editor::Render()
  {
    if(m_IsOpen)
    {
	m_Rocket->Render();
    }
  }
};
