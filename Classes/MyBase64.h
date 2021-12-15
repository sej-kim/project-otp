#include <string>
#ifdef  __cplusplus
extern "C" {
#endif


std::string base64_encode(unsigned char const*, unsigned int len);
std::string base64_decode(std::string const& s);
#ifdef  __cplusplus
}
#endif
