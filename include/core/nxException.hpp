#ifndef NEXUS_CORE_EXCEPTION_HPP
#define NEXUS_CORE_EXCEPTION_HPP

#include "../platform/nxPlatform.hpp"
#include <string>

namespace nexus { namespace core {

    class NEXUS_API NexusException : public std::exception
    {
      private:
        std::string fullErrorMessage;

      public:
        NexusException(const std::string& className, const std::string& errorMessage, const std::string& externalLibName = "", const std::string& externalLibError = "")
        {
            fullErrorMessage = className + ": " + errorMessage;

            if (!externalLibName.empty())
            {
                fullErrorMessage += " " + externalLibName + ": " + externalLibError;
            }
        }

        virtual const char* what() const noexcept override
        {
            return fullErrorMessage.c_str();
        }
    };

}}

#endif //NEXUS_CORE_EXCEPTION_HPP
