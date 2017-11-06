#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
#include <fantom/graphics.hpp>
#include <string>

using namespace fantom;

namespace {
    // my classes
    template<class T, size_t N>//somehow asks for a template to initialize
    class Integrator {
        public:
            double stepWidth;
            //virtual Tensor<T, N> nextStep(Tensor<T, N> xn, TensorFieldContinuous<3, Tensor<double, 3>>::Evaluator& evaluator) = 0;
            Integrator(double stepWidth){
                this->stepWidth = stepWidth;
                this->reset();
            }
            void reset(){}
            virtual ~Integrator(){} //initialize self; virtual to pass on inheritance
    };

    template<class T, size_t N>//somehow asks for a template to initialize
    class Euler: public Integrator<T, N>{
        public:
            Euler(double stepWidth) : Integrator<T, N>(stepWidth){}
            ~Euler(){}
    };

    template<class T, size_t N>//somehow asks for a template to initialize
    class RungeKutta: public Integrator<T, N>{
        public:
            RungeKutta(double stepWidth) : Integrator<T, N>(stepWidth){}
             ~RungeKutta(){}
    };

    //main class
    class Aufgabe4Algorithm: public VisAlgorithm{
        std::unique_ptr< Primitive > mGlyphs;

        public:
            struct Options : public VisAlgorithm::Options {
                Options( Control& control ) : VisAlgorithm::Options(control) {
                    add<TensorFieldContinuous<3, Vector3 > >("Field", "A 3D vector field");
                    add< double >("stepWidth", "steps size made", 1.0);
                    add< InputChoices >("method", "Chose a wizard to do the magic", std::vector<std::string>({"Euler", "RungeKutta"}), "Euler");  //default Euler
                }
            };

            struct VisOutputs : public VisAlgorithm::VisOutputs {
                VisOutputs( fantom::VisOutputs::Control& control ): VisAlgorithm::VisOutputs( control ){
                    addGraphics( "Glyphs" );
                }
            };

            Aufgabe4Algorithm( InitData& data ) : VisAlgorithm( data ) {
            }

            void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override {
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


                // here it begins
                //const ValueArray< Point3 >& points = myGrid->points();
                //std::vector< Vector3 > vertices(myGrid->numCells());
                //auto evaluator = myField->makeEvaluator();

                Integrator<double, 3> myIntegrator(options.get<double>("stepWidth"));

                //choose which function
                std::string method = options.get< std::string >("method");
                if (method.compare("Euler")) myIntegrator = Euler<double, 3>(options.get< double >("stepWidth"));
                else if (method.compare("RungeKutta")) myIntegrator = RungeKutta<double, 3>(options.get< double >("stepWidth"));



                //mGlyphs->add( Primitive::LINES ).setColor(Color(1,1,0)).setVertices( vertices ); //draw lines
            }
    };

    AlgorithmRegister< Aufgabe4Algorithm > reg("praktikum/Aufgabe4", "Science and more science. Actually a grid with flowvectors.");
}
