#include "core.hpp"

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "NumCraft";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;


int main(int argc, char * argv[]) {
  Core core = Core();
  
  core.loop();
  return 0;
}
