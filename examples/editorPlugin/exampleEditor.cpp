#include "exampleEditor.h"
#include "ExampleEditor_npk.h"

MINGED_PLUGIN_START_IMPLEMENT(ExampleEditor)
{
    // this is where you do your code
    MEngine::getInstance()->loadPlugin("Editor");
}

MINGED_PLUGIN_END_IMPLEMENT(ExampleEditor)
{
  // cleanup here
}
