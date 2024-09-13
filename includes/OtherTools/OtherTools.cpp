#include "OtherTools.h"

OtherTools::OtherTools()
{
}

void OtherTools::PrintIntro() {
  std::cout << "AESDumpster 1.2.5 - By GHFear @ IllusorySoftware" << std::endl;
  std::cout << "Supports Unreal Engine 4.19 -> 5.3 | (Will soon support UE 4.0 - 4.18 as well)\n\n" << std::endl;
}

void OtherTools::PrintInstructions() {
  std::cout << L"Usage:\n";
  std::cout << L"-Drag and drop Unreal Engine executables onto AESDumpster.exe.\n";
  std::cout << L"-Wait for the tool to finish.\n";
  //std::cin.ignore();
}

void OtherTools::PrintFileName(const char* argv) {
  std::cout << argv << std::endl;
}

void OtherTools::PrintOutro() {
  std::cout << "Done!\n";
}

int OtherTools::CreateExeBuffer(std::string filepath) {
  
  // Open file (exe).
  FILE* exe_file = fopen(filepath.c_str(), "rb");
  if (exe_file == NULL) {
    assert("Error opening exe file");
    return 1;
  }

  // Save start position.
  unsigned long long exe_start_pos = ftello64(exe_file);
  // Save archive size and go back to start position.
  fseeko64(exe_file, 0, SEEK_END);
  unsigned long long exe_size = ftello64(exe_file);
  fseeko64(exe_file, exe_start_pos, SEEK_SET);

  // Create buffer.
  char* file_buffer = (char*)malloc(exe_size);
  if (file_buffer == NULL) {
    assert("Error allocating memory");
    return 1;
  }

  // Read file into buffer.
  fread(file_buffer, exe_size, 1, exe_file);

  //Fill struct return values.
  retval.buffer = file_buffer;
  retval.size = exe_size;

  return 0;
}

OtherTools::~OtherTools()
{
}
