/*
Created by Sun-lay Gagneux
Project : La grande course
*/

#include "../include/ExampleClass.h"

/*
Include your header.
*/

#include <algorithm>
#include <vector>
/*
... Other include you do not need in the header like Algorithm, cmath...
*/



using namespace laGrandeCourse::exampleModule;

using std::for_each;
using std::vector;



const float Example::PUBLIC_CONSTANT_FLOAT_EXAMPLE = 105.3f; // In a float, do not forget f. You can do 10.3f, 0.f, 0.0f, 10.f, 106.0f...




void Example::someLongMethodWithoutParameter()
{
    /************************************************************************/
    /* IF, ELSE, ELSE IF EXAMPLE                                            */
    /************************************************************************/

    if (false /*condition*/)
    {
        return;
    }
    else if (true)
    {
        //Do something
    }
    else
    {
        //Do another thing...
    }



    /************************************************************************/
    /* FOR EXAMPLE                                                          */
    /************************************************************************/

    for (int iter = 0; iter < 1000; iter++)
    {
        // Do something
    }


    
    /************************************************************************/
    /* WHILE EXAMPLE                                                        */
    /************************************************************************/

    int nb = 0;

    while (nb < 1000)
    {
        // Do something
        nb++;
    }



    /************************************************************************/
    /* DO WHILE EXAMPLE                                                     */
    /************************************************************************/

    int cmpt = 0;

    do 
    {
        cmpt++;
    } while (cmpt < 1000);




    /************************************************************************/
    /* FOR_EACH AND LAMBDA EXAMPLE                                          */
    /************************************************************************/
    
    vector<anInt> exampleVector = { 100, 1, 52, -10, 2 };

    for_each(exampleVector.begin(), exampleVector.end(), [&](anInt val) {
        // Do something
    });



    /************************************************************************/
    /* SWITCH EXAMPLE                                                       */
    /************************************************************************/

    stateExampleEnum::childEnum state = stateExampleEnum::NONE;

    // Do something with state

    switch (state)
    {
    case stateExampleEnum::FIRST_STATE:
        //Do something
        break;

    case stateExampleEnum::SECOND_STATE:
    case stateExampleEnum::THIRD_STATE:
        //Do something
        break;

    default:
        throw FirstException{};
    }
}


Example::anInt Example::doSomethingWithParametersAndReturnAnInt(anInt someInt, aFloat someFloat /*, ...*/)
{
    // TODO
    return 0;
}


Example::anInt Example::somePrivateMethod()
{
    // TODO
    return 0;
}


Example::aFloat Example::someOtherPrivateMethod(aFloat valueA, anInt valueB)
{
    // TODO
    return 0.f;
}


Example& Example::operator=(const Example& otherExample)
{
    // TODO
    return *this;
}


bool Example::operator==(const Example& otherExample)
{
    return ((m_privateStandardIntMember == otherExample.m_privateStandardIntMember) &&
            (m_privateUsingFloatMember == otherExample.m_privateUsingFloatMember) &&
            (m_privateUsingIntMember == otherExample.m_privateUsingIntMember) &&
            (m_publicStandardIntMember == otherExample.m_publicStandardIntMember) &&
            (m_publicUsingFloatMember == otherExample.m_publicUsingFloatMember) &&
            (m_publicUsingIntMember == otherExample.m_publicUsingIntMember));
}


bool Example::operator<(const Example& otherExample)
{
    // TODO
    return true;
}


bool Example::operator>(const Example& otherExample)
{
    // TODO
    return true;
}
