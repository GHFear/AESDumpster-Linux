// AESDumpster 1.2.5 by GHFear

#include "../OtherTools/OtherTools.h"
#include "../KeyTools/KeyDumpster.h"

// Debug main logic
void debugmain()
{
  // Debug exe path (set your own path to an exe on your ssd here or this won't work)
  std::string exe_path = "Z:/Exes/NotProtected/SessionGame-Win64-Shipping.exe";

  // Print filename (exe full path).
  OtherTools* other_tools = new OtherTools();
  other_tools->PrintFileName(exe_path.c_str());

  //Create filebuffer using the given filepath.
  if (other_tools->CreateExeBuffer(exe_path.c_str()) != 0) { return; }
  if (other_tools->retval.buffer == nullptr) { printf("retval.buffer == nullptr.\n"); return; }

  // Create KeyDumpster object, scan for keys and print them.
  KeyDumpster* key_dumpster = new KeyDumpster();
  if (!key_dumpster->FindAESKeys(other_tools->retval.buffer, other_tools->retval.size))
    printf("There were no keys to be found or a problem occurred.\n");
  else
    key_dumpster->PrintKeyInformation();

  // Destruct KeyDumpster
  key_dumpster->~KeyDumpster();
  // Free buffer when done with it.
  free(other_tools->retval.buffer);
  // Print outro before exiting.
  other_tools->PrintOutro();
  // Destruct OtherTools
  other_tools->~OtherTools();

  return;
}

// Release main logic
void releasemain(int argc, char* argv[])
{
  OtherTools* other_tools = new OtherTools();
 
  // Print intro.
  other_tools->PrintIntro();

  if (argc == 1) {
    // Print instructions if launched without command.
    other_tools->PrintInstructions();
    return;
  }

  // Main loop.
  for (int i = 0; i < argc; i++) {
    if (i > 0) {
      // Print filename (exe full path).
      other_tools->PrintFileName(argv[i]);
      // Create filebuffer using the given filepath.
      if (other_tools->CreateExeBuffer(argv[i]) != 0) { return; }
      if (other_tools->retval.buffer == nullptr) { printf("retval.buffer == nullptr.\n");  break; }

      // Create KeyDumpster object, scan for keys and print them.
      KeyDumpster* key_dumpster = new KeyDumpster();
      if (!key_dumpster->FindAESKeys(other_tools->retval.buffer, other_tools->retval.size))
        printf("There were no keys to be found or a problem occurred.\n");
      else
        key_dumpster->PrintKeyInformation();

      // Destruct KeyTool object
      key_dumpster->~KeyDumpster();
      // Free buffer when done with it.
      free(other_tools->retval.buffer);
    }
  }
  // Print outro before exiting.
  other_tools->PrintOutro();
  // Destruct OtherTools object
  other_tools->~OtherTools();

  return;
}

// Entrypoint
int main(int argc, char* argv[])
{

#if defined _DEBUG
  debugmain();
#else
  releasemain(argc, argv);
#endif

  //std::cin.ignore();
  return 0;
}