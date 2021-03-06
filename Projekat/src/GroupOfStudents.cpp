//============================================================================
// File Name	   : GroupOfStudents.cpp
// Authors		   : Milos Panic, Dejan Todorovic
// Created         : 29.11.2019.
// Last Modified   : 02.12.2019. By Milos Panic
// Version         : 1.0
// Description     : Ovaj klasa predstavlja kolekciju klasa StudentCourses
// i sadrzi funckije za rad nad kolekcijom kao sto su trazenje najboljih,
// prikaz studenata, prikaz sortiranih studenata, citanje i pisanje u
// fajlove.
//============================================================================

#include "GroupOfStudents.h"

/**
 * Podrazumevani konstruktor.
 */
GroupOfStudents::GroupOfStudents()
{
}

/**
 * Konstruktor sa parametrima.
 */
GroupOfStudents::GroupOfStudents(const std::vector<StudentCourses>& v) :
	st_vec(v)
{
}

/**
 * Konstruktor kopije.
 */
GroupOfStudents::GroupOfStudents(const GroupOfStudents& gof) :
	st_vec(gof.st_vec)
{
}

/**
 * Funckija proverava da li medju ucitanim studentima ima
 * dva sa istim brojem indexa.
 *
 * @return true u slucaju da ima dva ili vise studenta
 * sa istim brojem indexa u suprotnom false ako su svi
 * jedinstveni.
*/
bool GroupOfStudents::check_for_duplicates() const
{
	std::set <std::string> student_ids;

	for (StudentCourses sc : st_vec)
	{
		student_ids.insert(sc.get_student().get_id());
	}

	return student_ids.size() != st_vec.size();
}

/**
 * Getter za student_courses.
 */
const std::vector<StudentCourses>& GroupOfStudents::get_student_courses() const
{
	return st_vec;
}

/**
 * Funkcija ispisuje sve studente zajedno sa njihovim ocenama na standardnom ulazu.
 * Ispis za svakog studenta je u formatu:
 * br_indexa ime prezime zavrsna_ocena znakovna_reprezentacija_ocene
*/
void GroupOfStudents::display() const
{
	std::cout << *this;
}

/**
 * Funkcija ispisuje sve studente zajedno sa njihovim ocenama na standardnom ulazu.
 * Studenti su sortirani prema alfabetnom redu i to prvo prezime, ime pa 
 * broj indexa.
 * Ispis za svakog studenta je u formatu:
 * br_indexa ime prezime zavrsna_ocena znakovna_reprezentacija_ocene
*/
void GroupOfStudents::display_sorted() const
{
	const std::vector<StudentCourses> vector_copy(st_vec);

	MergeSort::merge_sort<StudentCourses>(st_vec, alphabetical_comparator);
	display();
	st_vec = vector_copy;
}

/**
 * Funkcija ispisuje studenate sa najboljim prosekom.
 */
void GroupOfStudents::display_highest() const
{
	const double max_score = std::max_element(st_vec.begin(), st_vec.end(), grades_comparator)->get_final_score();

	for (StudentCourses sc : st_vec)
	{
		if (sc.get_final_score() == max_score)
		{
			sc.display();
		}
	}
}

/**
 * Funkcija ispisuje sve studente zajedno sa njihovim ocenama u izlazni tok.
 * Studenti su sortirani prema broju indexa
 * Ispis za svakog studenta je u formatu:
 * br_indexa ime prezime zavrsna_ocena znakovna_reprezentacija_ocene
 *
 * @param out - izlazni tok na kojem se cuvaju studenti.
*/
void GroupOfStudents::write_to_file(std::ofstream& out) const
{
	const std::vector<StudentCourses> vector_copy(st_vec);

	MergeSort::merge_sort<StudentCourses>(st_vec, id_comparator);

	out << *this;

	st_vec = vector_copy;
}

/**
 * Funkcija ispisuje sve studente zajedno sa njihovim ocenama u izlazni tok
 * u binarnom zapisu. Studenti su sortirani prema broju indexa
 * Ispis za svakog studenta je u formatu:
 * br_indexa ime prezime zavrsna_ocena znakovna_reprezentacija_ocene
 *
 * @param out - izlazni tok na kojem se cuvaju studenti.
*/
void GroupOfStudents::write_to_binary_file(std::ofstream& out) const
{
	const std::vector<StudentCourses> vector_copy(st_vec);

	MergeSort::merge_sort<StudentCourses>(st_vec, id_comparator);

	for (StudentCourses sc : st_vec)
	{
		sc.write_to_binary_file(out);
	}

	st_vec = vector_copy;
}

/**
 * Funkcija ucitava sve studente iz binarne textualne datoteke.
 * Jedan student je opisan sa redom velicine sizeof(Student) +
 * sizeof(int)*20 i smesta ih u st_vec.
 *
 * @param in - ulazni tok sa kojeg se citaju studenti.
*/
void GroupOfStudents::read_from_binary_file(std::ifstream& in) const
{
	st_vec.clear();

	while (!in.eof())
	{
		StudentCourses sc;

		sc.read_from_binary_file(in);

		if (in.fail())
		{
			if (in.eof())
			{
				in.clear();
			}

			break;
		}

		st_vec.push_back(sc);
	}
}

/**
 * Funkcija proverava da li u vektoru st_vec postoji student ciji je
 * broj indexa isti kao i zadatom. Ako je student pronadjen onda se
 * vraca pokazivac na njega a ako nije onda se vraca nullprt;
 *
 * @param id - broj indexa koji se trazi.
*/
StudentCourses* GroupOfStudents::find_by_id(const std::string& id) const
{
	const auto condition = [&id](const StudentCourses& sc) { return sc.get_student().get_id() == id; };
	const std::vector<StudentCourses>::iterator it = std::find_if(st_vec.begin(), st_vec.end(), condition);

	return (it != st_vec.end()) ? (&*it) : (nullptr);
}

/**
 * Funckija ispisuje podatke o studentu sa zadatim indexom. U slucaju
 * da ne postoji student sa zadatim indexom ispisace odgovarajucu
 * poruku o tome.
 *
 * @param id - broj indexa koji se trazi.
*/
void GroupOfStudents::display_one_student(const std::string& id) const
{
	const StudentCourses* s = find_by_id(id);

	if (s != nullptr)
	{
		std::cout << "Student sa zadatim indexom je:\n";
		s->display();
	}
	else
	{
		std::cout << "Ne postoji student sa unetim indeksom!\n";
	}
}

/**
 * Preklapanje operatora << za ispis svih studenata na izlaze.
 * Ispis je u formatu:
 * br_indexa ime prezime zavrsna_ocena znakovna_reprezentacija_ocene
 *
 * @param out - izlaz na koji se ispisuju podaci.
 * @param gof - objekat kolekcije StudentCourses ciji se podaci ispisuju.
 *
 */
std::ostream& operator <<(std::ostream& out, const GroupOfStudents& gof)
{
	for (StudentCourses sc : gof.st_vec)
	{
		out << sc;
	}

	return out;
}

/**
 * Funckija ucitava studente i njegove bodove dokle god ih ima u datoteci
 * ili dok ne dodje do neke greske. Sve ucitane studente smesta u st_vec.
 *
 * @param in - ulazni tok sa kojeg se citaju podaci.
 * @param gof - objekat kolekcije StudentCourses u koji se upisuju podaci.
*/
std::istream& operator >>(std::istream& in, GroupOfStudents& gof)
{
	gof.st_vec.clear();

	while (!in.eof())
	{
		StudentCourses sc;

		in >> sc;

		if (in.fail())
		{
			break;
		}

		gof.st_vec.push_back(sc);
	}

	return in;
}