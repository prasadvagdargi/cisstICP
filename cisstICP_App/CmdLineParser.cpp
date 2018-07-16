/*
Copyright (c) 2006, Michael Kazhdan and Matthew Bolitho.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution. 

Neither the name of the Johns Hopkins University nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include "CmdLineParser.h"


#ifdef WIN32
int strcasecmp(char* c1,char* c2){return _stricmp(c1,c2);}
#endif

////////////////////////////////////////////////////////////////////////////////
/*! Constructs a command line readable record with a given long and short name
//  @param[in]  name        long option name
//  @param[in]  shortName   short option name
//                          defaults to none (-1)
*///////////////////////////////////////////////////////////////////////////////
cmdLineReadable::cmdLineReadable(const char *name, char shortName)
{
	set=false;
	this->name=strdup(name);
    this->shortName = shortName;
    this->description = strdup("NO DESCRIPTION: %s\n");
}

////////////////////////////////////////////////////////////////////////////////
/*! Sets the description string for this argument (used for usage printout)
//  @param[in] desc     the description string. Should be in the format:
//                      "... %s ... [%s] ... \n" %s is the name placeholder,
//                      and %s is the value string placeholder
*///////////////////////////////////////////////////////////////////////////////
void cmdLineReadable::setDescription(const char *desc)
{
    free(description);
    description = strdup(desc);
}

////////////////////////////////////////////////////////////////////////////////
/*! Prints the description string for this argument
*///////////////////////////////////////////////////////////////////////////////
void cmdLineReadable::printDescription()
{
    char val[512];
    this->writeValue(val);
    printf(description, name, val);
}

cmdLineReadable::~cmdLineReadable(void)
{
	if(name) free(name);
    free(description);
    shortName = -1;
	name=NULL;
}
int cmdLineReadable::read(char**,int){
	set=true;
	return 0;
}
void cmdLineReadable::writeValue(char* str)
{
	str[0] = 0;
}

////////////////
// cmdLineInt //
////////////////
cmdLineInt::cmdLineInt(const char *name, char shortName)
    : cmdLineReadable(name, shortName), value(0) { }
cmdLineInt::cmdLineInt(const char *name, const int &v, char shortName)
    : cmdLineReadable(name, shortName), value(v) { }
int cmdLineInt::read(char** argv,int argc){
	if(argc>0){
		value=atoi(argv[0]);
		set=true;
		return 1;
	}
	else{return 0;}
}
void cmdLineInt::writeValue(char* str)
{
	sprintf(str,"%d",value);
}

////////////////////////
// cmdLineIntSequence //
////////////////////////
cmdLineIntSequence::cmdLineIntSequence(const char *name, char shortName)
	: cmdLineReadable(name, shortName) { }
cmdLineIntSequence::cmdLineIntSequence(const char *name, int v, char shortName)
	: cmdLineReadable(name, shortName), value(v),
	  start(v), end(v), increment(1) { }
int cmdLineIntSequence::read(char **argv, int argc)
{
	if (argc > 0)	{
		const char *argString = argv[0];
		set = true;
		int assigned = sscanf(argString, "%d:%d:%d", &start, &increment, &end);
		if (assigned != 3)	{
			increment = 1;
			assigned = sscanf(argString, "%d:%d", &start, &end);
			if (assigned != 2)	{
				assigned = sscanf(argString, "%d", &start, &end);
				end = start;
				if (assigned != 1)	{
					start = value;
					set = false;
				}
			}
		}

		reset();
		return 1;
	}
	return 0;
}
void cmdLineIntSequence::writeValue(char *str)
{
	sprintf(str, "%i:%i:%i", start, increment, end);
}

/////////////////
// cmdLineInts //
/////////////////
cmdLineInts::cmdLineInts( const char* name , char shortName ) : cmdLineReadable( name , shortName )
{
	values = NULL;
	count = 0;
}
cmdLineInts::~cmdLineInts(void)
{
	if(values)	delete[] values;
	values = NULL;
	count = 0;
}
int cmdLineInts::read(char** argv,int argc)
{
	if(argc>0)
	{
		count=atoi(argv[0]);
		if(count <= 0 || argc <= count)	return 1;
		values = new int[count];
		if(!values)	return 0;
		for ( int i = 0 ; i < count ; i++ )	values[i] = atoi ( argv[i+1] );
		set=true;
		return count + 1;
	}
	else return 0;
}
void cmdLineInts::writeValue(char* str)
{
	char* temp=str;
	for( int i=0 ; i<count ; i++ )
	{
		sprintf(temp,"%d ",values[i]);
		temp=str+strlen(str);
	}
}

//////////////////
// cmdLineFloat //
//////////////////
cmdLineFloat::cmdLineFloat(const char *name, char shortName)
    : cmdLineReadable(name, shortName), value(0) { }
cmdLineFloat::cmdLineFloat(const char *name, const float &v, char shortName)
    : cmdLineReadable(name, shortName), value(v) { }
int cmdLineFloat::read(char** argv,int argc){
	if(argc>0){
		value=(float)atof(argv[0]);
		set=true;
		return 1;
	}
	else{return 0;}
}
void cmdLineFloat::writeValue(char* str)
{
	sprintf(str,"%f",value);
}

///////////////////
// cmdLineString //
///////////////////
cmdLineString::cmdLineString(const char *name, char shortName)
    : cmdLineReadable(name, shortName), value(NULL) { }
cmdLineString::~cmdLineString(void)
{
    free(value);
	value=NULL;
}
int cmdLineString::read(char** argv,int argc){
	if(argc>0)
	{
        free(value);
		value=strdup(argv[0]);
		set=true;
		return 1;
	}
	else{return 0;}
}
void cmdLineString::writeValue(char* str)
{
    if (value != NULL)
        sprintf(str,"%s",value);
}

////////////////////
// cmdLineStrings //
////////////////////
cmdLineStrings::cmdLineStrings(const char *name, char shortName)
    : cmdLineReadable(name, shortName)
{
	values = NULL;
	count = 0;
}
cmdLineStrings::~cmdLineStrings(void)
{
	for( int i=0 ; i<count ; i++ )
		if( values[i] )	delete[] values[i] , values[i] = NULL;
	delete[] values;
	values = NULL;
	count = 0;
}
int cmdLineStrings::read(char** argv,int argc)
{
	if( argc>0 )
	{
		count = atoi( argv[0] );
		if( count <= 0 || argc <= count ) return 1;
		values = new char*[count];
		if( !values ) return 0;
		for( int i = 0 ; i < count ; i++ ) values[i] = new char[ strlen( argv[i+1] )+1 ] , strcpy( values[i] , argv[i+1] );;
		set=true;
		return count + 1;
	}
	else return 0;
}
void cmdLineStrings::writeValue(char* str)
{
	char* temp=str;
	for( int i=0 ; i<count ; i++ )
	{
		sprintf( temp , "%s " , values[i] );
		temp = str+strlen(str);
	}
}

char* FileExtension( char* fileName )
{
	char* temp = fileName;
	for( int i=0 ; i<strlen(fileName) ; i++ ) if( fileName[i]=='.' ) temp = &fileName[i+1];
	return temp;
}

char* GetFileExtension( char* fileName )
{
	char* fileNameCopy;
	char* ext=NULL;
	char* temp;

	fileNameCopy=new char[strlen(fileName)+1];
	assert(fileNameCopy);
	strcpy(fileNameCopy,fileName);
	temp=strtok(fileNameCopy,".");
	while(temp!=NULL)
	{
		if(ext!=NULL){delete[] ext;}
		ext=new char[strlen(temp)+1];
		assert(ext);
		strcpy(ext,temp);
		temp=strtok(NULL,".");
	}
	delete[] fileNameCopy;
	return ext;
}
char* GetLocalFileName( char* fileName )
{
	char* fileNameCopy;
	char* name=NULL;
	char* temp;

	fileNameCopy=new char[strlen(fileName)+1];
	assert(fileNameCopy);
	strcpy(fileNameCopy,fileName);
	temp=strtok(fileNameCopy,"\\");
	while(temp!=NULL){
		if(name!=NULL){delete[] name;}
		name=new char[strlen(temp)+1];
		assert(name);
		strcpy(name,temp);
		temp=strtok(NULL,"\\");
	}
	delete[] fileNameCopy;
	return name;
}
char* LocalFileName( char* fileName )
{
	char* temp = fileName;
	for( int i=0 ; i<strlen(fileName) ; i++ ) if( fileName[i] =='\\' ) temp = &fileName[i+1];
	return temp;
}
char* DirectoryName( char* fileName )
{
	for( int i=strlen(fileName)-1 ; i>=0 ; i-- )
		if( fileName[i] =='\\' )
		{
			fileName[i] = 0;
			return fileName;
		}
	fileName[0] = 0;
	return fileName;
}

////////////////////////////////////////////////////////////////////////////////
/*! Finds the param with a particular name.
//  @param[in]  params  list of parameters to match against (NULL-terminated)
//  @param[in]  name    name to search for
//  @return     pointer to the matching readable, or NULL if none was found
*///////////////////////////////////////////////////////////////////////////////
cmdLineReadable *getReadableByLongName(cmdLineReadable **params
                                     , const char *name)
{
    cmdLineReadable *opt = NULL;
    while (*params != NULL)   {
        if (strcmp(name, (*params)->name) == 0)   {
            opt = *params;
            break;
        }
        ++params;
    }
    return opt;
}

////////////////////////////////////////////////////////////////////////////////
/*! Finds the param with a particular short name.
//  @param[in]  params  list of parameters to match against (NULL-terminated)
//  @param[in]  c       short name to search for
//  @return     pointer to the matching readable, or NULL if none was found
*///////////////////////////////////////////////////////////////////////////////
cmdLineReadable *getReadableByShortName(cmdLineReadable **params, char c)
{
    cmdLineReadable *opt = NULL;
    while (*params != NULL)   {
        if ((*params)->shortName == c)   {
            opt = *params;
            break;
        }
        ++params;
    }
    return opt;
}

////////////////////////////////////////////////////////////////////////////////
/*! Parses a list of command line arguments. Arguments are expected in the
//  format:
//      [^-].*              non-option argument
//      --longName          long-named flag (no parameter)
//      --longName value    long-named parameter
//      --longName=value    long-named parameter
//      -abc                short-named flags without parameters (a, b, c)
//      -s value            short-named parameter
//      -svalue             short-named parameter
//  If non-null argcStripped and argvStripped are provided, only the matched
//  arguments are processed, and plain and unmatched arguments are placed in
//  this filtered output. This allows arguments to be parsed in multiple passes.
//
//  If the stripped output pointers aren't provided, unmatched arguments
//  trigger and error message, and plain arguments are pushed onto the
//  nonoptArgs vector.
//  @param[in]  argc        number of arguments
//  @param[in]  argv        argument strings
//  @param[in]  params      the parameters to match against (NULL-terminated)
//  @param[out] nonoptArgs  plain arguments without "-" or "--" before them
//  @param[out] argcStripped    number of Command line arguments with matched
//                              options stripped
//  @param[out] argvStripped    Command line arguments with matched options
//                              stripped. These should be deallocated with free
//                              when no longer needed.
*///////////////////////////////////////////////////////////////////////////////
void cmdLineParse(int argc, char **argv, cmdLineReadable** params,
        std::vector<std::string> &nonoptArgs, int *argcStripped,
        char ***argvStripped)
{
    // Whether we are stripping out the arguments we match so they can be parsed
    // for different options in the future
    bool stripping = (argcStripped != NULL);
    assert(!stripping || (argvStripped != NULL));
    std::vector<char *> strippedArgs;

    if (stripping)  {
        // Copy over the invocation path
        strippedArgs.push_back(strdup(argv[0]));
    }

    // Move past the invocation path
    --argc, ++argv;

	int i,j;
	while (argc > 0)
	{
        if (argv[0][0] == '-')  {
            if (argv[0][1] == '-')  {
                const char *currentOption = argv[0];

                // Match a long option
                argv[0] += 2;
                std::string name(argv[0]);
                size_t eqPos = name.find('=');
                if (eqPos != name.npos)   {
                    name = name.substr(0, eqPos);
                    // make argv[0] point to option's argument
                    *argv += eqPos + 1;
                }
                else    {
                    // make argv[0] point to option's argument
                    // (or the next option if we don't need an argument)
                    ++argv, --argc;
                }
                cmdLineReadable *opt = getReadableByLongName(params
                                                           , name.c_str());
                if (opt != NULL)    {
                    if (opt->expectsArg())  {
                        j = opt->read(argv, argc);
                        // Jump past the read arguments to the next option
                        argv += j, argc -= j;
                    }
                    else    {
                        if (eqPos != name.npos) {
                            // What is the '=' doing here!?
                            fprintf(stderr
                                  , "[WARNING] unexpected argument for option %s\n"
                                  , name.c_str());
                        }
                        else    {
                            // Argument-less options just need to be set
                            opt->set = true;
                        }
                    }
                }
                else    {
                    if (!stripping) {
                        fprintf(stderr, "[WARNING] invalid option: --%s\n"
                              , currentOption);
                    }
                    else    {
                        strippedArgs.push_back(strdup(currentOption));
                    }

                    if (eqPos != name.npos) {
                        // If this was an argument with an "=", we still must
                        // advance to the next option
                        ++argv, --argc;
                    }
                }
            }
            else    {
                char c;
                // Match clumps of short options (if they don't expect arguments)
                while ((c = *(++argv[0])))    {
                    // *argv[0] holds the unprocessed option or argument
                    // character after the current short name in c
                    cmdLineReadable *opt = getReadableByShortName(params, c);
                    if (opt != NULL)    {
                        if (!(opt->expectsArg()))  {
                            opt->set = true;
                            continue;
                        }
                        else    {
                            ++argv[0];
                            if (*argv[0])   {
                                j = opt->read(argv, argc);
                            }
                            else    {
                                ++argv, --argc;
                                j = opt->read(argv, argc);
                            }

                            // Jump past read arguments to just before the next
                            // option
                            argv += j - 1, argc -= j - 1;

                            // the clump of short options has been terminated by
                            // the argument
                            break;
                        }
                    }
                    else    {
                        if (!stripping)
                            fprintf(stderr, "[WARNING] invalid option: -%c\n", c);
                        else    {
                            char *option = strdup("-c");
                            option[1] = c;
                            strippedArgs.push_back(option);
                        }
                    }
                }

                // Advance to the next option
                ++argv, --argc;
            }
        }
		else
		{
            // Plain arguments are passed along untouched if we are stripping
            // the matched arguments. Otherwise they are placed in the
            // nonoptArgs vector.
            if (stripping)
                strippedArgs.push_back(strdup(*argv));
            else
                nonoptArgs.push_back(std::string(*argv));
            ++argv, --argc;
		}
	}

    if (stripping)  {
        *argcStripped = strippedArgs.size();
        *argvStripped = (char **) malloc(strippedArgs.size() * sizeof(char *));
        std::copy(strippedArgs.begin(), strippedArgs.end(), *argvStripped);
    }
}

char** ReadWords(const char* fileName,int& cnt)
{
	char** names;
	char temp[500];
	FILE* fp;

	fp=fopen(fileName,"r");
	if(!fp){return NULL;}
	cnt=0;
	while(fscanf(fp," %s ",temp)==1){cnt++;}
	fclose(fp);

	names=new char*[cnt];
	if(!names){return NULL;}

	fp=fopen(fileName,"r");
	if(!fp){
		delete[] names;
		cnt=0;
		return NULL;
	}
	cnt=0;
	while(fscanf(fp," %s ",temp)==1){
		names[cnt]=new char[strlen(temp)+1];
		if(!names){
			for(int j=0;j<cnt;j++){delete[] names[j];}
			delete[] names;
			cnt=0;
			fclose(fp);
			return NULL;
		}
		strcpy(names[cnt],temp);
		cnt++;
	}
	fclose(fp);
	return names;
}
