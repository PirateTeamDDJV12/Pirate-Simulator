/*
    Created by Sun-lay Gagneux
    Project : La grande course
*/
#ifndef EXAMPLE_H
#define EXAMPLE_H


/*

#include "YourPersonalInclude" 
...

#include <LibraryInclude>
...

*/



/*
DO NOT DO in the include:
using namespace whatever;
*/



namespace laGrandeCourse // project name
{
    namespace exampleModule // not mandatory if the class isn't part of any feature
    {
        /*
        * Some doc about your class. Tell us what this class is for and where is the unit test that tests this class.
        */
        class Example
        {
        private: // Not mandatory since it is private by default
            class PrototypeNotDefinedHere;
            class SecondPrototypeNotDefinedHere;



        public:
            using anInt = int;
            using aFloat = float;



        public:
            /* 
            * Comment 
            */
            static const int PUBLIC_CONSTANT_INTEGER_EXAMPLE = 1000;

            /*
            * Comment. Give its value
            */
            static const float PUBLIC_CONSTANT_FLOAT_EXAMPLE;



        protected:
            /*
            * Comment
            */
            static const int PROTECTED_CONSTANT_INTEGER_EXAMPLE = 500;



            /*
            // I don't know if your into private constants but I think it is useless.
            private:
                static const int PRIVATE_CONSTANT_INTEGER_EXAMPLE = 1000;
            */


        public:
            struct stateExampleEnum
            {
                enum childEnum
                {
                    NONE = 0,
                    FIRST_STATE = 1,
                    SECOND_STATE = 2,
                    THIRD_STATE = 3
                }aliasIfYouWant, secondAliasIfYouWant /*, etc*/; // Possible but not mandatory
            };

            class actionExampleEnum
            {
            public:
                enum otherChildEnum
                {
                    NONE = 0,
                    MOVE_EXAMPLE_ACTION = 1,
                    FIRE_EXAMPLE_ACTION = 2,
                    FOCUS_EXAMPLE_ACTION = 3
                };
            };

            enum
            {
                BORDER_MIN = 0,
                BORDER_MAX = 1000,
            };

            /*
            Good C++ tips :
            Here, you can use a internal struct, a namespace or even an internal class that sole purpose is to hold the enum.
            Like this, you can have a same good name into 2 enum (typically a NONE, DEFAULT, ...).

            IT IS CLEANER THAN using name like NONE_STATE or NONE_EXAMPLE_ACTION just because you can't use 2 enums
            whith the same key name inside

            go read : http://gamesfromwithin.com/stupid-c-tricks-2-better-enums
            It is a really good game designer and programmer that wrote an article on this.
            It has many other tricks and it is not so long to peek at those articles.
            */


        public:
            class FirstException {};
            class SecondException {};

            struct PublicInternalStruct
            {
                /* Same pattern */
            };

            class PublicInternalClass
            {
                /* Same pattern */
            };



        protected:
            struct ProtectedInternalStruct
            {
                /* Same pattern */
            };

            class ProtectedInternalClass
            {
                /* Same pattern */
            };



        private:
            struct PrivateInternalStruct
            {
                /* Same pattern */
            };

            class PrivateInternalClass
            {
                /* Same pattern */
            };



        public:

            /*
            * Comment
            */
            anInt m_publicUsingIntMember;

            /*
            * Comment
            */
            aFloat m_publicUsingFloatMember;

            /*
            * Comment
            */
            int m_publicStandardIntMember;

            /*etc.*/



        private:
            anInt m_privateUsingIntMember;
            aFloat m_privateUsingFloatMember;
            int m_privateStandardIntMember;

            /*etc.*/



        public:
            /*
            * Comment.
            */
            Example() :
                m_publicStandardIntMember{},
                m_publicUsingFloatMember{},
                m_publicUsingIntMember{},
                m_privateStandardIntMember{},
                m_privateUsingFloatMember{},
                m_privateUsingIntMember{}
            {}

            /*
            * Comment. What is a, b, c, ...? DO NOT do what I've done => a,b,c isn't really explicitly good names, name correctly your parameters please.
            */
            Example(anInt a, aFloat b, int c /*, etc*/) :
                m_publicStandardIntMember{ a },
                m_publicUsingFloatMember{ b },
                m_publicUsingIntMember{ c },
                m_privateStandardIntMember{/*etc*/ },
                m_privateUsingFloatMember{/*etc*/ },
                m_privateUsingIntMember{/*etc*/ }
            {}

            Example(const Example& copy) :
                m_publicStandardIntMember{ copy.m_publicStandardIntMember },
                m_publicUsingFloatMember{ copy.m_publicUsingFloatMember },
                m_publicUsingIntMember{ copy.m_publicUsingIntMember },
                m_privateStandardIntMember{ copy.m_privateStandardIntMember },
                m_privateUsingFloatMember{ copy.m_privateUsingFloatMember },
                m_privateUsingIntMember{ copy.m_privateUsingIntMember }
            {}

            ~Example()
            {
                /*2 lines maximum, if not => .cpp*/
            }



        public:

            /*
            * - It isn't mandatory to do some doc to methods without parameter, getter and setter if methods names are explicit
            * - Even if the method is explicit, its parameters aren't. Thus, do some comment
            * - For getter and setter, it isn't mandatory to name the method with get and set as prefixes but be explicit !!
            */

            anInt getPrivateUsingIntMember() const noexcept
            {
                return m_privateUsingIntMember;
            }

            aFloat getPrivateUsingFloatMember() const noexcept
            {
                return m_privateUsingFloatMember;
            }

            int getPrivateStandardIntMember() const noexcept
            {
                return m_privateStandardIntMember;
            }

            void setPrivateUsingIntMember(anInt newValue) noexcept
            {
                m_privateUsingIntMember = ((newValue > BORDER_MAX) ? BORDER_MAX : ((newValue < BORDER_MIN) ? BORDER_MIN : newValue));
                // 2 level ternary operator maximum. More ? Think anew your code architecture or do some if, else if, else or switch in the cpp
            }

            void setPrivateUsingFloatMember(aFloat newValue) noexcept
            {
                m_privateUsingFloatMember = newValue;
            }

            void setPrivateStandardIntMember(int newValue) noexcept
            {
                m_privateStandardIntMember = newValue;
            }

            /*
            * Comment if the name isn't explicit
            */
            void someTwoLinesMethodWithoutParameter() noexcept
            {
                // Do something within 2 lines maximum
            }

            /*
            * Comment if the name isn't explicit
            */
            void someLongMethodWithoutParameter();

            /*
            * Here, the name is explicit so no comment allowed...
            */
            bool isSomethingAccordingToTheClassStateLikeIsEmptyForExample() const noexcept
            {
                return true; /*TODO : What you want within the 2 lines range rule please, if not : cpp*/
            }


            /*
            * Please comment what someInt, someFloat are AND if it isn't explicit, what the returned value is
            */
            anInt doSomethingWithParametersAndReturnAnInt(anInt someInt, aFloat someFloat /*, ...*/);



        private:
            anInt somePrivateMethod();

            aFloat someOtherPrivateMethod(aFloat valueA, anInt valueB);

            aFloat someTwoLinePrivateMethod()
            {
                // Line 1
                return PUBLIC_CONSTANT_FLOAT_EXAMPLE; // In a float, do not forget f. You can do 10.3f, 0.f, 0.0f, 10.f, ...
            }



        public:
            Example& operator=(const Example& otherExample);

            bool operator==(const Example& otherExample);

            bool operator!=(const Example& otherExample)
            {
                return !(*this == otherExample); // Gift
            }

            bool operator<(const Example& otherExample);

            bool operator>(const Example& otherExample);

            bool operator<=(const Example& otherExample)
            {
                return !(*this > otherExample); // Gift
            }

            bool operator>=(const Example& otherExample)
            {
                return !(*this < otherExample); // Gift
            }
            
            /*...*/
        };
    }
}



#endif //!EXAMPLE_H