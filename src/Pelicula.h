#pragma once
#include <string>
#include <sstream>


class Pelicula
{
private:
	// Atributos
	std::string title;
	int year;

	double WWE; // Worldwide Earnings: Ingresos en la taquilla a nivel mundial.
	double DE;  // Domestic Earnings: Ingresos en la taquilla a nivel nacional.
	double FE;  // Foreign Earnings: Ingresos en la taquilla a nivel internacional

	double DPE; // Domestic Percent Earnings: Ingresos porcentuales en la 
				// taquilla a nivel nacional respecto alos ingresos mundiales.

	double FPE; // Foreign   Percent   Earnings: Ingresos   porcentuales   
				// en   la   taquilla   a   nivel internacionalrespecto a los ingresos mundiales.

public:

	Pelicula(std::string title, int year, double wwe, double de, double fe, double dpe, double fpe);

	inline std::string getTitle() const { return title; }
	inline int getYear() const { return year; }
	inline double getWWE() const { return WWE; }
	inline double getDE() const { return DE; }
	inline double getFE() const { return FE; }
	inline double getDPE() const { return DPE; }
	inline double getFPE() const { return FPE; }

	// Sobrecargar el operador
	friend std::ostream& operator<<(std::ostream& os, const Pelicula& pelicula);

	bool operator==(const Pelicula& other) const;
	bool operator==(const std::string other) const;

	bool operator!=(const Pelicula& other) const;
	bool operator!=(const std::string other) const;

	bool operator<(const Pelicula& other) const;
	bool operator<(const std::string other) const;

	bool operator>(const Pelicula& other) const;
	bool operator>(const std::string other) const;

	bool operator>=(const Pelicula& other) const;
	bool operator>=(const std::string other) const;

	bool operator<=(const Pelicula& other) const;
	bool operator<=(const std::string other) const;
};

