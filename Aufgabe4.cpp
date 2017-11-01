#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
#include <fantom/graphics.hpp>

using namespace fantom;

namespace {
    class Integrator : public VisAlgorithm {

    };

    class Aufgabe4Algorithm : public VisAlgorithm {
        std::unique_ptr< Primitive > mGlyphs;//universe of images

    public:
        struct Options : public VisAlgorithm::Options {
            Options( Control& control ) : VisAlgorithm::Options(control) {
                // add options here
                add<TensorFieldContinuous<3, Vector3 > >("Field", "A 3D vector field");

            }
        };

        Aufgabe4Algorithm( InitData & data) : VisAlgorithm(data) {

        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override {
            //add your code here
            mGlyphs = getGraphics("Glyphs").makePrimitive();
            std::shared_ptr< const TensorFieldContinuous< 3, Vector3 > > myField = options.get< TensorFieldContinuous< 3, Vector3 > >( "Field" );

            // if there is no input, do nothing
            if( !myField ) {
                debugLog() << "Input Field not set." << std::endl;
                return;
            }
            // sanity check that interpolated fields really use the correct grid type. This should never fail
            std::shared_ptr< const Grid< 3 > > myGrid = std::dynamic_pointer_cast< const Grid< 3 > >( myField->domain() );
            if( !myGrid ) {
                throw std::logic_error( "Wrong type of grid!" );
            }

            const ValueArray< Point3 >& points = myGrid->points();
            std::vector< Vector3 > vertices(myGrid->numCells());
            auto evaluator = myField->makeEvaluator();

            for( int i = 0; myGrid ->numCells(); i++){
                Cell cell = myGrid->cell( i );    //calculates barrycenter; not needed here but for seeing how to do it
                Point3 point;

                for( size_t j = 0; j != cell.numVertices(); ++j )
                {
                    point += points[cell.index( j )];
                }
                point /= cell.numVertices();

                if( evaluator->reset( point, time ) ) { //returns True if point inside the domain
                    auto v = evaluator->value(); // v is the value returned by the evaluator
                    locMaximum = std::max( locMaximum, norm( v ) );
                    vertices[i] = point;
                    vertices[i + 1] = v;
                }



            }

            mGlyphs->add( Primitive::LINES ).setColor(Color(1,0,1)).setVertices( vertices ); //draw it

        }
    };

    AlgorithmRegister< Aufgabe4Algorithm > reg("praktikum/Aufgabe4", "Science and more science. Actually a grid with flowvectors.");
}
