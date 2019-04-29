#pragma once

#include <memory>

namespace bdn::net
{
    class HTTPRequest;
    class HTTPResponse;

    namespace http
    {
        enum class Method
        {
            GET,
            POST,
            PUT,
            DELETE,
            HEAD,
            TRACE,
            OPTIONS,
            CONNECT
        };

        std::shared_ptr<HTTPResponse> request(HTTPRequest request);
    }
}
