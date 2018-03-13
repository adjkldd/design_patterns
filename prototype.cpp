// Purpose.  Prototype design pattern lab
//
// Problem.  See the problem statement for the Factory Method lab.  Beyond
// the issues discussed there, we would also like to eliminate the "case"
// statement that currently exists in main().  We can do that by replacing
// the conditional code with a "table-driven" design.
//
// Assignment.
// o Add a second pure virtual function to class FederalFraudvillian -
//      virtual FederalFraudvillian* clone() = 0;
// o Create a Factory base class with a single "virtual constructor" -
//      FederalFraudvillian* createPrevaricator( PrevaricatorType type )
// o Add the following enum declaration to the Factory class -
//      enum PrevaricatorType { None, PresT, AGT, MPCPT };
// o Add a protected array of FederalFraudvillian "prototypical instances" 
//   to the Factory class -
//      FederalFraudvillian* prototypes[4];
// o The body of the createPrevaricator() method will simply use its type
//   argument as an offset into the prototypes array, call clone() on that
//   prototypical instance, and return whatever clone() returns
// o Create two classes derived from Factory: DemocratFactory, and
//   RepublicanFactory
// o These two derived classes will only contain a default constructor.
//   Each constructor will populate the prototype array in the base class
//   with instances of the classes: 0, President, AttorneyGeneral, and
//   MinorityPartyCongressPerson.  Make sure these "prototypes" are of the
//   correct party affiliation.
// o Instantiate a factory object at the top of main() with code like -
//      #ifdef DEMOCRAT
//         Factory* factory = new DemocratFactory;
//      #else
//         Factory* factory = new RepublicanFactory;
//      #endif
// o Replace the calls to "new" in main() with a single call to -
//      createPrevaricator( in )
// o The "new"s that were being performed in main() will now be done
//   [indirectly] in the clone() method of each FederalFraudvillian derived
//   class
// o Add a 1-arg constructor to the FederalFraudvillian derived classes.
//   Use a "member initialization list" to map the char* argument the
//   constructor receives to the Base class's 1-arg constructor.
// o Add a clone() method to the FederalFraudvillian derived classes.  The
//   body of clone() should create an instance of the class it is a member of
//   (being careful to supply the char* argument the constructor is expecting)
//   and return the new instance.
// o Move as many declarations and definitions as possible below main() to
//   demonstrate the minimal coupling that remains

#include <iostream>
#include <string>
#include <cstring>

using namespace std;

class FederalFraudvillian {
 public:
   FederalFraudvillian( const char* pp = "Democrat" ) { strcpy( party, pp ); }
   virtual void partyLine() = 0;
   virtual FederalFraudvillian* clone() = 0;
 protected:
   char party[20];
};

class President: public FederalFraudvillian {
 public:
   President(const char* pp = "Republican" ): FederalFraudvillian(pp) {}
   void partyLine() { cout << party << ": speak no evil" << endl; }
   FederalFraudvillian* clone() {
     return new President(party);
   }
};

class AttorneyGeneral : public FederalFraudvillian {
 public:
   AttorneyGeneral(const char* pp = "Republican" ): FederalFraudvillian(pp) {}
   void partyLine() { cout << party << ": see no evil" << endl; }
   FederalFraudvillian* clone() {
     return new AttorneyGeneral(party);
   }
};

class MinorityPartyCongressPerson : public FederalFraudvillian {
 public:
   MinorityPartyCongressPerson(const char* pp = "Republican" ): FederalFraudvillian(pp) {}
   void partyLine() { cout << party << ": hear no evil" << endl; }
   FederalFraudvillian* clone() {
     return new MinorityPartyCongressPerson(party);
   }
};

class Factory {
 protected:
   FederalFraudvillian* prototypes[4];
 public:
   enum PrevaricatorType { None, PresT, AGT, MPCPT };

   FederalFraudvillian* createPrevaricator( PrevaricatorType type ) {
     FederalFraudvillian* f = prototypes[type];
     return f->clone();
   }

   ~Factory() {
     for (int i = 1; i < sizeof(prototypes)/sizeof(prototypes[0]); i++) {
       delete prototypes[i];
     }
   }
};

class DemocratFactory: public Factory {
 public:
   DemocratFactory() {
     prototypes[0] = 0;
     prototypes[1] = new President("Democrat");
     prototypes[2] = new AttorneyGeneral("Democrat");
     prototypes[3] = new MinorityPartyCongressPerson("Democrat");
   }
};

class RepublicanFactory: public Factory {
 public:
   RepublicanFactory() {
     prototypes[0] = 0;
     prototypes[1] = new President("Republican");
     prototypes[2] = new AttorneyGeneral("Republican");
     prototypes[3] = new MinorityPartyCongressPerson("Republican");
   }
};

int main( void ) {
#ifdef DEMOCRAT
   Factory* factory = new DemocratFactory;
#else
   Factory* factory = new RepublicanFactory;
#endif
   FederalFraudvillian*  moralMisfits[10];
   int      in, i, total = 0;

   cout << "President(1) AG(2) MPCP(3) Go(0): ";
   cin >> in;
   while (in) {
      moralMisfits[total++] = factory->createPrevaricator(static_cast<Factory::PrevaricatorType>(in));
      cout << "President(1) AG(2) MPCP(3) Go(0): ";
      cin >> in;
   }
   for (i=0; i < total; i++) moralMisfits[i]->partyLine();
   for (i=0; i < total; i++) delete moralMisfits[i];
 
   delete factory;
   return 0;
}

// no compiler directive -
// President(1) AG(2) MPCP(3) Go(0): 1
// President(1) AG(2) MPCP(3) Go(0): 2
// President(1) AG(2) MPCP(3) Go(0): 3
// President(1) AG(2) MPCP(3) Go(0): 1
// President(1) AG(2) MPCP(3) Go(0): 0
// Republican: speak no evil
// Republican: see no evil
// Republican: hear no evil
// Republican: speak no evil

// -DDEMOCRAT -
// President(1) AG(2) MPCP(3) Go(0): 1
// President(1) AG(2) MPCP(3) Go(0): 2
// President(1) AG(2) MPCP(3) Go(0): 3
// President(1) AG(2) MPCP(3) Go(0): 1
// President(1) AG(2) MPCP(3) Go(0): 0
// Democrat: speak no evil
// Democrat: see no evil
// Democrat: hear no evil
// Democrat: speak no evil
