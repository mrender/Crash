#include "File.h"
#include "../Services/Services.h"
#include "../Windows/Application.h"
#include <fstream>

namespace GameDev2D
{
    File::File(const std::string& aPath)
    {        
        //Check to see if the file exists, if it doesn't the assert below will be hit
        bool doesExist = Services::GetApplication()->DoesFileExistAtPath(aPath);
        assert(doesExist == true);
        
        //Does the file exist
        if(doesExist == true)
        {
            //Open the input stream for the file
            std::ifstream inputFile(aPath.c_str());
            
            //Read line by line the contents of the file,
            //while there is still content to load
            while(inputFile.good() == true)
            {
                std::string line;
                getline(inputFile, line);
                m_FileText.append(line + "\n");
            }
        }
    }
    
    File::~File()
    {
        
    }
    
    const char* File::GetBuffer()
    {
        return m_FileText.c_str();
    }
    
    unsigned long File::GetBufferSize()
    {
        return m_FileText.length();
    }
}
