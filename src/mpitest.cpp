#include "mpitest.h"

#include <iostream>
#include <sstream>

#include <mpi.h>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/mpi.hpp>

// Setups MPI and allows single binaries to be run in parallel of one another.
void HelloMPI(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    std::cout << "Enter a number: ";
    int n;
    std::cin >> n;

    for (int i = 1; i < size; ++i) {
      MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  } else {
    int n;
    MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << 10 << std::endl;
  }
  MPI_Finalize();
}

struct Person {
  friend class boost::serialization::access;

  std::string name;
  int age;

  /**
   * Serialize the Person struct into a char*
   */
  template<class Archive> 
  void serialize(Archive& archive, const unsigned int version) {
    archive& name;
    archive& age;
  }


};

void MPIWithObjects(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Person p;
    p.name = "Dimitri";
    p.age = 5000;

    // Serialize the person struct using boost archives.
    std::ostringstream os;
    boost::archive::text_oarchive archive(os);
    archive << p;

    std::string serialized_person = os.str();
    int len = serialized_person.length() + 1;

    for (int i = 1; i < size; i++) {
      MPI_Send(serialized_person.c_str(), len, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }

  } else {
    MPI_Status status;

    // Probe data until it's received and then get the size of incoming data.
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    int count;
    MPI_Get_count(&status, MPI_CHAR, &count);
    std::cout << "We have received " << count << " bytes" << std::endl;

    // Knowing the size of buffer,read it into memory.
    char* buffer  = new char[count];
    MPI_Recv(buffer, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    std::cout << "We received our buffer: " << buffer << std::endl;

    // Convert the buffer into an input stream and then deserialize it.
    std::istringstream iss(buffer);
    boost::archive::text_iarchive archive(iss);
    Person p;
    archive >> p;

    std::cout 
      << "Hello, " 
      << p.name 
      << ", you are" 
      << p.age 
      << " years old" 
      << std::endl; 

    delete[] buffer;
  }
 
  MPI_Finalize();
}

void MPIWithBoost(int argc, char* argv[]) {

  namespace mpi = boost::mpi;

  mpi::environment env;
  mpi::communicator world;

  if (world.rank() == 0) {
    Person p;
    p.name = "Yeeter";
    p.age = 2300;

    for (int i = 1; i < world.size(); i++) {
      world.send(i, 0, p);
    }
  } else {
    Person p;
    world.recv(0, 0, p);
    std::cout 
      << p.name << " is my name and I'm " << p.age << " years old" << std::endl;
  }
}

void DifferentMPISends(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    int value = 42;
    for (int i = 1; i < size; i++) {
      std::cout << "Ready to send from " << rank << " --> " << i << std::endl;
      MPI_Ssend(&value, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  } else {
    int received;
    MPI_Recv(&received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }


  MPI_Finalize();
}

void RunMPIExamples(int argc, char* argv[]) {
  // HelloMPI(argc, argv);
  // MPIWithObjects(argc, argv);
  MPIWithBoost(argc, argv);
}
