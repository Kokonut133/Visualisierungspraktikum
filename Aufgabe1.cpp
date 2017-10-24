#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/math.hpp>
#include <fantom/fields.hpp>

using namespace fantom;

namespace
{
    class Aufgabe1 : public DataAlgorithm
    {
        //bool isAutoRun = true;  //reloads options when changes occur

    public:
        struct Options : public DataAlgorithm::Options
        {
            Options( fantom::Options::Control& control ) : DataAlgorithm::Options(control)
            {
                // add options here
                add< int >( "Number of houses" , "Specify how many houses you want" , 1 );
            }
        };

        struct DataOutputs : public DataAlgorithm::DataOutputs
        {
            DataOutputs( fantom::DataOutputs::Control& control )
                : DataAlgorithm::DataOutputs( control )
            {
                add< Grid< 3 > >( "grid" );
            }
        };

        Aufgabe1( InitData & data) : DataAlgorithm(data)
        {
            // initialize internal data members
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {

            Tensor<double, 3> p0(1.0, 0.0, 0.0);
            Tensor<double, 3> p1(0.0, 1.0, 0.0);
            Tensor<double, 3> p2(0.0, 0.0, 1.0);

            std::vector<Tensor<double,3>> myTriangle;
            myTriangle.push_back(p0);
            myTriangle.push_back(p1);
            myTriangle.push_back(p2);

            for (std::vector<Tensor<double,3 > >::const_iterator i = myTriangle.begin(); i != myTriangle.end(); ++i)
                debugLog() << *i << ' ' ;

            std:: shared_ptr <const DiscreteDomain< 3 > > myDomain = DomainFactory::makeDomainArbitrary(std::move(myTriangle), Precision::UINT64); //punkte in dem Universum


            //int indices[3] = {0,1,2};


            //std::shared_ptr< const Grid< 3 > > m = fantom::DomainFactory::makeGridUnstructured( myDomain, 1, cellCounts, indices ); // parent domain, numCellTypes, cellCounts
            debugLog() << std::endl;

        }
    };

    AlgorithmRegister< Aufgabe1 > reg("VisPraktikum/Aufgabe1", "Created Data for tiny houses. Displayable with grid");
}
