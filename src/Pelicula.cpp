#include "Pelicula.h"

Pelicula::Pelicula(std::string title, int year, double wwe, double de, double fe, double dpe, double fpe) 
	: title(title), year(year), WWE(wwe), DE(de), FE(fe), DPE(dpe), FPE(fpe) {
}

bool Pelicula::operator==(const Pelicula& other) const
{
	if (
		this->title == other.title &&
		this->year == other.year &&
		this->WWE == other.WWE &&
		this->DE == other.DE &&
		this->FE == other.FE &&
		this->DPE == other.DPE &&
		this->FPE == other.FPE
		)
		return true;
	else
		return false;
}

bool Pelicula::operator==(const std::string other) const
{
	if (title == other)
		return true;
	else
		return false;
}

bool Pelicula::operator!=(const Pelicula& other) const
{
	return title != other.title;
}

bool Pelicula::operator!=(const std::string other) const
{
	return title != other;
}

bool Pelicula::operator<(const Pelicula& other) const
{
	return title < other.title;
}

bool Pelicula::operator<(const std::string other) const
{
	if (this->title < other)
		return true;
	else
		false;
}


bool Pelicula::operator>(const Pelicula& other) const
{
	return title > other.title;
}

bool Pelicula::operator>(const std::string other) const
{
	return title > other;
}

bool Pelicula::operator<=(const Pelicula& other) const
{
	return !(*this > other);
}

bool Pelicula::operator<=(const std::string other) const
{
	return title > other;
}

bool Pelicula::operator>=(const Pelicula& other) const
{
	return !(*this < other);
}

bool Pelicula::operator>=(const std::string other) const
{
	return title < other;
}

std::ostream& operator<<(std::ostream& os, const Pelicula& pelicula)
{
	os << pelicula.title;
	return os;
}
