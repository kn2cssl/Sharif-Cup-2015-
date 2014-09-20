#include "shape.h"

Shape::Shape()
{
    //position = new Vector2D();
    IsValid=false;
}

void Shape::set(Vector2D position, double roundedRadios, std::string color, std::string type)
{
    this->position=position;
    this->roundedRadios=roundedRadios;
    this->color=color;
    this->type=type;

}
