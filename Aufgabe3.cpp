#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
#include <fantom/graphics.hpp>


using namespace fantom;

namespace {
    class Aufgabe3Algorithm : public VisAlgorithm {
        std::unique_ptr< Primitive > mGlyphs;

    public:
        struct Options : public VisAlgorithm::Options {
            Options( Control& control ) : VisAlgorithm::Options(control)
            {
                add< TensorFieldDiscrete < Scalar > >("Field", "Input as Scalar" );
                add< double >("Threshold", "Above this value, it is made visible", 0.0008);
            }
        };

        struct VisOutputs : public VisAlgorithm::VisOutputs {
            VisOutputs( fantom::VisOutputs::Control& control ) : VisAlgorithm::VisOutputs( control ) {
                addGraphics( "Glyphs" );
            }
        };

        Aufgabe3Algorithm( InitData & data) : VisAlgorithm(data) {
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override {
            mGlyphs = getGraphics("Glyphs").makePrimitive();
            std::shared_ptr <const TensorFieldDiscrete <Scalar> > myField = options.get<TensorFieldDiscrete <Scalar> >("Field");

            //checken ob es ein Feld gibt
            if(!myField){
                return;
            }

           //punkte einlesen
            std::shared_ptr< const Grid< 3 > > grid = std::dynamic_pointer_cast< const Grid< 3 > >( myField -> domain() );
            auto& points = grid->points();

            //evaluator und leeres Ergebnis Array schaffen
            std::unique_ptr< TensorFieldDiscrete< Scalar >::DiscreteEvaluator> myEvaluator = myField->makeDiscreteEvaluator();
            std::vector< Point3 > visibleValues;

            //alle punkte durchgehen
            double myThreshold = options.get<double>("Threshold");
            int count = 0;
            for (size_t i = 0; i < points.size(); i++){
                auto thisValue = myEvaluator->value(i);
                if(thisValue[0] >= myThreshold){
                    //debugLog() << points[i] << std::endl;
                    visibleValues.push_back(points[i]);
                    count++;
                }
            }

            //punkte zeichnen
            mGlyphs -> add(Primitive::POINTS).setPointSize(10).setVertices(visibleValues);

        }
    };

    AlgorithmRegister< Aufgabe3Algorithm > reg("praktikum/Aufgabe3", "Shows points for a scalarfield if it surpasses a value");
}
