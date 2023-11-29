#include "UI.h"

UI::UI()
{

};

std::string UI::HyphenGenerator(int amountOfHyphens)
{
    if(amountOfHyphens > 100)
    {
        amountOfHyphens = 100;
    }
    std::string hyphens = "";
    for(int i=0; i<amountOfHyphens; i++)
    {
        hyphens+= "-";
    }
    return hyphens;
}

std::string UI::WhiteSpaceOffScreen()
{
    std::string whitespace = "";
    for(int i=0; i<20; i++)
    {
        whitespace += '\n';
    }

    return whitespace;
}