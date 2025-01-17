#include "HTTPResponseMessage.hpp"

const std::map<int, const std::string> HTTPResponseMessage::responseStatusMessages =
        {
                { 200, "OK" },                        /* SUCCESS */
                { 201, "Created" },
                { 202, "Accepted" },
                { 204, "No Content" },

                { 301, "Moved Permanently" },        /* REDIRECTION */
                { 302, "Found" },
                { 303, "See Other" },
                { 307, "Temporary Redirect" },

                { 400, "Bad Request" },                /* CLIENT ERRORS */
                { 403, "Forbidden" },
                { 404, "Not Found" },
                { 405, "Method Not Allowed" },
                { 408, "Request Timeout" },
                { 410, "Gone" },
                { 411, "Length Required" },
                { 413, "Payload Too Large" },
                { 414, "URI Too Long" },
                { 418, "I'm a teapot" },
                { 429, "Too Many Requests" },
                { 431, "Request Header Fields Too Large" },

                { 500, "Internal Server Error" },    /* SERVER ERRORS */
                { 501, "Not Implemented" },
                { 505, "HTTP Version Not Supported" }
                // ...
        };

const std::string HTTPResponseMessage::toString() const {
    std::string output;

    output =
            protocol + ' ' + _getStatusCodeStr() + ' ' + message + "\r\n" +

            _headerContentLengthToString() +
            _headerContentTypeToString() +
            _headerDateToString() +
            _headerLocationToString() +
            _headerServerToString() +
            "\r\n" +

            body;

    return output;
}

HTTPResponseMessage& HTTPResponseMessage::addStatus( const e_responseStatusCode status ) {
    this->status = status;
    this->message = responseStatusMessages.at( static_cast<int>(status) );
    return * this;
}

HTTPResponseMessage& HTTPResponseMessage::addType( const std::string type ) {
    this->type = type;
    return * this;
}

HTTPResponseMessage& HTTPResponseMessage::addLength( const unsigned int contentLength ) {
    length = contentLength;
    return * this;
}

HTTPResponseMessage& HTTPResponseMessage::addBody( const std::string body ) {
    this->body = body;
    return * this;
}

HTTPResponseMessage& HTTPResponseMessage::addLocation( const std::string location ) {
    this->location = location;
    return * this;
}

HTTPResponseMessage::e_responseStatusCode HTTPResponseMessage::getStatus() const{
    return this->status;
}

HTTPResponseMessage& HTTPResponseMessage::closeConnection( void ){
    this->_closeConnection = true;
    return * this;
}

const std::string HTTPResponseMessage::_headerConnection( void ) const {
    if ( _closeConnection )
        return "connection: Closed\r\n";
    else
        return "";
}

const std::string HTTPResponseMessage::_headerContentLengthToString( void ) const {
    // if ( body != "" )
    return "content-length: " + std::to_string( length ) + "\r\n";
    // else
    //     return "";
}

const std::string HTTPResponseMessage::_headerContentTypeToString( void ) const {
    if ( body != "" )
        return "content-type: " + type + "\r\n";
    else
        return "";
}

const std::string HTTPResponseMessage::_headerDateToString( void ) const {
    return "date: " + _getDateStr() + "\r\n";
}

const std::string HTTPResponseMessage::_headerLocationToString( void ) const {
    if ( location != "" )
        return "location: " + location + "\r\n";
    else
        return "";
}

const std::string HTTPResponseMessage::_headerServerToString( void ) const {
    return "server: " + server + "\r\n";
}

std::string HTTPResponseMessage::_getDateStr() const {
    time_t rawTime;
    struct tm* timeInfo;
    char buffer[128];

    time(&rawTime);
    timeInfo = gmtime(&rawTime);

    strftime(buffer, 128,"%a, %d %b %G %T GMT", timeInfo);

    std::string output = static_cast<std::string>(buffer);

    return output;
}

const std::string HTTPResponseMessage::_getStatusCodeStr() const {
    return std::to_string( static_cast<int>(status) );
}
