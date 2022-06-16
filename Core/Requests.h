#pragma once
#include "core.h" // core should be included first

#include <string>
#include <map>

KSINETWORK_START
namespace Core {

   using HttpParams = std::multimap<std::string, std::string>;
   using HttpHeaders = std::multimap<std::string, std::string>;

   enum class HttpMethod{
      // we need only GET and POST
      GET,
      POST
   };

   std::string Request(
      const std::string& url,
      const HttpParams& params      = {},
      HttpMethod method             = HttpMethod::GET,
      const HttpHeaders& headers    = {},
      const std::string& postData   = ""
   );

}
KSINETWORK_END