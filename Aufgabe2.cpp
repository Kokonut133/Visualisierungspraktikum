#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
#include <fantom/graphics.hpp>

using namespace fantom;

namespace
{
    class Aufgabe2Algorithm : public VisAlgorithm
    {
        std::unique_ptr< Primitive > mGlyphs;//universe of images

    public:
        struct Options : public VisAlgorithm::Options
        {
            Options( Control& control ) : VisAlgorithm::Options( control )
            {
                add< Grid < 3 > >("Grid", "Zu zeichnendes Grid" );
                add< Color >("Color", "Zeichenfarbe", Color(0.75, 0.75, 0.0));
                add< bool >("Show single cell", "Nur eine einzelne Zelle anzeigen", false);
                add< unsigned int >("Number of cell", "Nummer der anzuzeigenden Zelle", 0);
                setVisible("Number of cell", false);
            }

            void optionChanged( const std::string& name ) {
                if (name == "Show single cell") {
                    bool value = get< bool >("Show single cell");
                    setVisible("Number of cell", value);
                }
            }
    };

        struct VisOutputs : public VisAlgorithm::VisOutputs
        {
            VisOutputs( fantom::VisOutputs::Control& control ) : VisAlgorithm::VisOutputs( control )
            {
                addGraphics( "Glyphs");
            }
        };

        Aufgabe2Algorithm( InitData & data) : VisAlgorithm( data )
        {
        }

        void drawMyType(std::shared_ptr<const Grid<3>>& grid, unsigned int& index, const Color& color) {
            Cell c = grid->cell(index);
            Cell::Type cellType = c.type();
            const ValueArray<Tensor<double,3>>& points = grid->points(); //getting the points from my grid

            switch (cellType) {
                case Cell::Type::PYRAMID: {
                    //Bottom
                    std::vector<Tensor<double,3>> bottom;
                    bottom.push_back(points[c.index(0)]);
                    bottom.push_back(points[c.index(1)]);
                    bottom.push_back(points[c.index(2)]);
                    bottom.push_back(points[c.index(3)]);

                    //Triangles
                    std::vector<Tensor<double,3>> vertices;
                    std::vector<std::vector<int>> sides;
                    sides.push_back(std::vector<int>({4,1,0}));
                    sides.push_back(std::vector<int>({4,2,1}));
                    sides.push_back(std::vector<int>({4,3,2}));
                    sides.push_back(std::vector<int>({4,0,3}));

                    for (unsigned int i = 0; i < sides.size(); i++) {
                        for (unsigned int j = 0; j < sides[i].size(); j++) {
                            vertices.push_back(points[c.index(sides[i][j])]);
                        }
                    }
                    mGlyphs->add(Primitive::QUADS).setColor(color).setVertices(bottom);
                    mGlyphs->add(Primitive::TRIANGLES).setColor(color).setVertices(vertices);
                    return;
                }
                case Cell::Type::HEXAHEDRON: {
                    //sides
                    std::vector<Tensor<double,3>> vertices;
                    std::vector<std::vector<int>> sides;
                    sides.push_back(std::vector<int>({0,7,6,1}));//front
                    sides.push_back(std::vector<int>({1,6,5,2}));//right
                    sides.push_back(std::vector<int>({2,5,4,3}));//behind
                    sides.push_back(std::vector<int>({3,4,7,0}));//left
                    sides.push_back(std::vector<int>({7,6,5,4}));//top
                    sides.push_back(std::vector<int>({0,1,2,3}));//bottom

                    for (unsigned int i = 0; i < sides.size(); i++) {
                        for (unsigned int j = 0; j < sides[i].size(); j++) {
                            vertices.push_back(points[c.index(sides[i][j])]);
                        }
                    }
                    mGlyphs->add(Primitive::QUADS).setColor(color).setVertices(vertices);
                    return;
                }
                case Cell::Type::LINE: {
                    //pole
                    std::vector<Tensor<double,3>> line;
                    line.push_back(points[c.index(0)]);
                    line.push_back(points[c.index(1)]);

                    mGlyphs->add(Primitive::LINES).setColor(Color(1,1,1)).setVertices(line);
                    return;
                }
                default: {
                    debugLog() << "Try again with a different type" << std::endl;
                    return;
                }
            }
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
            mGlyphs = getGraphics("Glyphs").makePrimitive();

            std::shared_ptr< const Grid < 3 > > grid = options.get< Grid < 3 > >("Grid");
            if (!grid) {
                debugLog() << "I need some kind of grid man" << std::endl;
                return;
            }

            //draw walls
            Color color = options.get<Color>("Color");
            if (options.get<bool>("Show single cell")) {
                auto cell = options.get< unsigned int >("Number of cell");
                drawMyType(grid, cell, color);
            }
            else {
                for (unsigned int cell = 0; cell < grid->numCells(); cell++) {
                    drawMyType(grid, cell, color);
                }
            }
        }

    };

    AlgorithmRegister< Aufgabe2Algorithm > reg("praktikum/Aufgabe2", "Fills out mah Buildings");
}
