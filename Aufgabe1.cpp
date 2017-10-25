#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/math.hpp>
#include <fantom/fields.hpp>
#include <fantom/dataset/ValueArray.hpp>

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

            //roof points
            Tensor<double, 3> r0(1.0, 0.0, 0.0);
            Tensor<double, 3> r1(0.0, 1.0, 0.0);
            Tensor<double, 3> r2(-1.0, 0.0, 0.0);
            Tensor<double, 3> r3(0.0, -1.0, 0.0);
            Tensor<double, 3> r4(0.0, 0.0, 1.0);

            std::vector<Tensor<double,3>> house;
            house.push_back(r0); // index 0
            house.push_back(r1);
            house.push_back(r2);
            house.push_back(r3);
            house.push_back(r4);

            //block floor points
            Tensor<double, 3> r5(1.0, 0.0, -1.0);
            Tensor<double, 3> r6(0.0, 1.0, -1.0);
            Tensor<double, 3> r7(-1.0, 0.0, -1.0);
            Tensor<double, 3> r8(0.0, -1.0, -1.0);

            house.push_back(r5); // index 5
            house.push_back(r6);
            house.push_back(r7);
            house.push_back(r8);

            //pole points
            Tensor<double, 3> r9(0.0,0.0,1.5);

            house.push_back(r9); //index 9

            //flag points
            Tensor<double, 3> r10(0.0, 0.0, 1.3);
            Tensor<double, 3> r11(0.0, 0.2, 1.4);

            house.push_back(r10); //index 10
            house.push_back(r11);


           // for (std::vector<Tensor<double,3 > >::const_iterator i = house.begin(); i != house.end(); ++i)
           //    debugLog() << *i << ' ' ; // shows me the tensors

            std::shared_ptr <const DiscreteDomain< 3 > > myDomain = DomainFactory::makeDomainArbitrary(std::move(house), Precision::UINT64); //punkte in dem Universum

            std::pair<Cell::Type, size_t> cellCounts[4];
            cellCounts[0] = std::pair<Cell::Type, size_t> (Cell::PYRAMID,1);
            cellCounts[1] = std::pair<Cell::Type, size_t> (Cell::HEXAHEDRON,1);
            cellCounts[2] = std::pair<Cell::Type, size_t> (Cell::LINE,1);
            cellCounts[3] = std::pair<Cell::Type, size_t> (Cell::TRIANGLE,1);

            std::vector< size_t > toUsePoints({0,1,2,3,4, 5,6,7,8,3,2,1,0, 9,4, 10,11,9}); //0-4 pyramid, 5,6,7,8,3,2,1,0 hexahedron, 9,4, line, 10, 11, 9 Triangle/flag

            std::shared_ptr< const Grid< 3 > > myGrid = DomainFactory::makeGridUnstructured( *myDomain, 4, cellCounts, toUsePoints ); // domain, numDifferentCellTypes, <typeOfForm, howOften>, whichPointsToUse
            setResult("grid", myGrid);

            debugLog() << std::endl;

        }
    };

    AlgorithmRegister< Aufgabe1 > reg("VisPraktikum/Aufgabe1", "Created Data for tiny houses. Displayable with grid");
}























