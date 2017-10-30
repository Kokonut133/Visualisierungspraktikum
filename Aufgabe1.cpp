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
                add< int >( "Number of Houses" , "Specify how many houses you want" , 1);
            }
        };

        struct DataOutputs : public DataAlgorithm::DataOutputs
        {
            DataOutputs( fantom::DataOutputs::Control& control ) : DataAlgorithm::DataOutputs( control )
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

            double houseNumber = (double)options.get<int>("Number of Houses");

            double shift = 3.0;
            std::vector<Tensor<double,3>> points;

            std::vector<size_t> indices;

            //Quadrat
            for (int i = 0; i < houseNumber; i++) {
                double mone = -1.0+(i*shift);
                double zero = 0.0+(i*shift);
                double one = 1.0+(i*shift);

                Tensor<double, 3> r0(one, 0.0, 0.0);
                Tensor<double, 3> r1(zero, 1.0, 0.0);
                Tensor<double, 3> r2(mone, 0.0, 0.0);
                Tensor<double, 3> r3(zero, -1.0, 0.0);
                Tensor<double, 3> r4(zero, -1.0, 1.0);
                Tensor<double, 3> r5(mone, 0.0, 1.0);
                Tensor<double, 3> r6(zero, 1.0, 1.0);
                Tensor<double, 3> r7(one, 0.0, 1.0);

                points.push_back(r0);
                points.push_back(r1);
                points.push_back(r2);
                points.push_back(r3);
                points.push_back(r4);
                points.push_back(r5);
                points.push_back(r6);
                points.push_back(r7);

                for(int j = 0; j<8; j++){
                    indices.push_back(j+(8*i));
                }
            }

            //Dach
            int count = 0;
            for (int i = 0; i < houseNumber; i++) {
                double zero = 0.0+(i*shift);

                Tensor<double, 3> r8(zero, 0.0, 1.5);

                points.push_back(r8);

                for(int j = 4+(i*8); j<(8+(i*8)); j++){
                    indices.push_back(j);
                }
                indices.push_back(8*houseNumber+count);
                count++;
            }

           //Stange
            int count2 = 0;
            for (int i = 0; i < houseNumber; i++){
                double zero = 0.0+(i*shift);

                Tensor<double, 3> r9(zero, 0.0, 1.7);

                points.push_back(r9);

                indices.push_back(8*houseNumber+count2);
                indices.push_back(8*houseNumber+houseNumber+count2);
                count2++;
            }


            //Zuordnung der Punkte zu Zellen
            size_t differentCellTypes = 3;

            std::pair<Cell::Type, size_t> cellCounts[differentCellTypes];
            size_t houses = (size_t) houseNumber;
            std::pair<Cell::Type, size_t> hexas(Cell::HEXAHEDRON, houses);
            std::pair<Cell::Type, size_t> pyramids(Cell::PYRAMID, houses);
            std::pair<Cell::Type, size_t> line (Cell::LINE , houses);
            cellCounts [0] = hexas;
            cellCounts [1] = pyramids;
            cellCounts [2] = line;


            /*
            for(int i = 0; i<=houseNumber ; i++){

                double mone = -1.0+(i*shift);
                double zero = 1.0+(i*shift);
                double one = 0.0+(i*shift);

                //roof points
                Tensor<double, 3> r0(one, 0.0, 0.0);
                Tensor<double, 3> r1(zero, 1.0, 0.0);
                Tensor<double, 3> r2(mone, 0.0, 0.0);
                Tensor<double, 3> r3(zero, -1.0, 0.0);
                Tensor<double, 3> r4(zero, 0.0, 1.0);

                house.push_back(r0); // index 0
                house.push_back(r1);
                house.push_back(r2);
                house.push_back(r3);
                house.push_back(r4);

                //block floor points
                Tensor<double, 3> r5(one, 0.0, -1.0);
                Tensor<double, 3> r6(zero, 1.0, -1.0);
                Tensor<double, 3> r7(mone, 0.0, -1.0);
                Tensor<double, 3> r8(zero, -1.0, -1.0);

                house.push_back(r5); // index 5
                house.push_back(r6);
                house.push_back(r7);
                house.push_back(r8);

                //pole points
                Tensor<double, 3> r9(zero,0.0,1.5);

                house.push_back(r9); //index 9

                //flag points
                Tensor<double, 3> r10(zero, 0.0, 1.3);
                Tensor<double, 3> r11(zero, 0.2, 1.4);

                house.push_back(r10); //index 10
                house.push_back(r11);

                int a = 0+(i*4);
                int b = 1+(i*4);
                int c = 2+(i*4);
                int d = 3+(i*4);

                cellCounts[a] = std::pair<Cell::Type, size_t> (Cell::PYRAMID,1);
                cellCounts[b] = std::pair<Cell::Type, size_t> (Cell::HEXAHEDRON,1);
                cellCounts[c] = std::pair<Cell::Type, size_t> (Cell::LINE,1);
                cellCounts[d] = std::pair<Cell::Type, size_t> (Cell::TRIANGLE,1);

            }

            for(int i  = 0; i<=pointsPerHouse; i++){
                indices.push_back(firstPoints[i]);
            }
            */

            std::shared_ptr <const DiscreteDomain< 3 > > myDomain = DomainFactory::makeDomainArbitrary(points, Precision::UINT64); //punkte in dem Universum

            // for debugging
            /*
            for (size_t i = 0; i < points.size(); i++){
               debugLog() << int(i) << " " << points[i] << std::endl; // shows me the tensors
            }
            for (size_t i = 0; i < indices.size(); i++){
               debugLog() << int(i) << " " << indices[i] << std::endl; // shows me the tensors
            }
            debugLog() << cellCounts << std::endl; // show me cellcount


            std::shared_ptr< const Grid< 3 > > myGrid = DomainFactory::makeGridUnstructured( *myDomain, differentCellTypes, cellCounts, indices ); // domain, numDifferentCellTypes, <typeOfForm, howOften>, whichPointsToUse
            setResult("grid", myGrid);
            */

        }
    };

    AlgorithmRegister< Aufgabe1 > reg("VisPraktikum/Aufgabe1", "Created Data for tiny houses. Displayable with grid");
}























