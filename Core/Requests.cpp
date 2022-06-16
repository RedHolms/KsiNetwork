#include "Requests.h"

#include <sstream>

#include <string.h>

#include <curl/curl.h>

#include "Exceptions.h"

static bool g_CurlInitialized = false;

static std::string normalizeStringForUrl(const std::string& str) {
   std::string result;
   for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
      char chr = *it;
      switch (chr) {
      case ' ':
      case '\n':
      case '\r': {
         std::stringstream stream;
         stream << "%" << std::hex << (((int)chr) & 0xFF);
         result.append(stream.str());
      } break;
      default:
         result.append(&chr, 1);
         break;
      }
   }
   return result;
}

static std::string serializeUrlParams(const KsiNetwork::Core::HttpParams& params) {
   char buff[4096]{ 0 };
   for (KsiNetwork::Core::HttpParams::const_iterator it = params.begin(); it != params.end(); it++) {
      if (it == params.begin()) strcat_s(buff, _countof(buff), "?");
      strcat_s(buff, _countof(buff), (normalizeStringForUrl(it->first) + "=" + normalizeStringForUrl(it->second)).c_str());
      if (it != --params.end()) strcat_s(buff, _countof(buff), "&");
   }
   return buff;
}

static size_t curlWriteCallback(char* ptr, size_t size, size_t nmemb, std::string* data) {
   if (data) {
      data->append(ptr, size * nmemb);
      return size * nmemb;
   }
   return 0;
}

static curl_slist* headersToCurlSlist(const KsiNetwork::Core::HttpHeaders& headers) {
   curl_slist* slist_headers = nullptr;
   for (auto& header : headers)
      slist_headers = curl_slist_append(slist_headers, (header.first + ": " + header.second).c_str());
   return slist_headers;
}

KSINETWORK_START
namespace Core {

   std::string Request(const std::string& url, const HttpParams& params, HttpMethod method, const HttpHeaders& headers, const std::string& postData) {
      if (!g_CurlInitialized) {
         assert(curl_global_init(CURL_GLOBAL_ALL) == CURLE_OK);
         g_CurlInitialized = true;
      }

      std::string errorBuffer; errorBuffer.resize(CURL_ERROR_SIZE + 1);
      std::string response;
      CURL* curl = curl_easy_init();
      assert(curl);

      std::string fullUrl = url + serializeUrlParams(params);

      curl_easy_setopt(curl, CURLOPT_URL, fullUrl.c_str());
      curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer.c_str());
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
      curl_easy_setopt(curl, CURLOPT_USERAGENT, "KsiNetwork");
      curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
      
      curl_slist* slist_headers = headersToCurlSlist(headers);
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist_headers);
      
      switch (method) {
         case HttpMethod::GET:
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
            break;
         case HttpMethod::POST:
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
            break;
      }

      CURLcode code = curl_easy_perform(curl);

      if (slist_headers)
         curl_slist_free_all(slist_headers);

      curl_easy_cleanup(curl);

      if (code != CURLE_OK)
         throw CurlFailedException(code, errorBuffer);

      return response;
   }

}
KSINETWORK_END