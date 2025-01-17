#include "configParser.hpp"

using namespace webserv;

webserv::configParser::configParser( std::string config_file ) {
    _configFile.open( config_file, std::fstream::in );
    _errorCode = NONE;
    if ( _configFile.fail() )
        _errorCode = NOFILE;
    else if ( _configFile.bad() )
        _errorCode = BADFILE;
    else if ( _configFile.eof() )
        _errorCode = EMPTYFILE;
    else {
        tokenizer();
        _it = _tokens.begin();
    }
    return;
}

webserv::configParser::~configParser( void ) {
    _configFile.close();
    return;
}

void webserv::configParser::tokenizer( void ) {
    std::string line;

    while ( getline( _configFile, line ) ) {
        for ( int i = 0; line[i]; i++ ) {
            if ( !isspace( line[i] ) )
                i += _newToken( & line[i] );
        }
    }
}

int webserv::configParser::_newToken( std::string line ) {
    std::string token;
    int i = 0;

    if ( line[i] == ';' || line[i] == '{' || line[i] == '}' )
        token.append( & line[i++], 1 );
    else
        do {
            if ( isspace( line[i] ) || line[i] == ';' || line[i] == '{' || line[i] == '}' )
                break;
            token.append( & line[i], 1 );
        } while ( line[++i] );
    _tokens.push_back( token );
    return --i;
}


int webserv::configParser::parseIntoPieces( socketData& socketData, httpData& httpData ) {
    TokenType::iterator it = _tokens.begin();
    int ret;

    if ( _it++->compare( "server" ) || * _it != "{" )
        return ERROR;
    while ( ++_it != _tokens.end() && * _it != "}" ) {
        if ( * _it == "listen" )
            ret = setSocket( socketData, httpData );
        else if ( * _it == "location" )
            ret = setLocation( httpData );
        else if ( * _it == "server_name" )
            ret = setServerName( httpData );
        else if ( * _it == "error_page" )
            ret = setErrorPage( httpData );
        else if ( * _it == "return" )
            ret = setRedirect( httpData );
        else if ( * _it == "client_max_body_size" )
            ret = setClientMaxBodySize( httpData );
        else if ( * _it == "}" )
            return SUCCES;
        else {
            _errorCode = SERVER;
            ret = ERROR;
        }
        if ( ret == ERROR )
            return ret;
    }
    httpData.organizeLocations();
    httpData.setErrorPages();
    if ( * ( _it++ ) == "}" && _it != _tokens.end() )
        return NEOF;
    return SUCCES;
}
