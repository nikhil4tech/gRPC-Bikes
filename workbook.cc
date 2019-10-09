// See README.txt for information and build instructions.

#include <ctime>
#include <fstream>
#include <google/protobuf/util/time_util.h>
#include <iostream>
#include <string>

#include "workbook.pb.h"

using namespace std;

using google::protobuf::util::TimeUtil;

// This function fills in a Person message based on user input.
void PromptForBikeDetails(nik_tutorial::Bike* bike) {
  
    /* -----------------------------------------------------------------
        Bike Details - Registration Number , Manufacturer Name and Model 
       ----------------------------------------------------------------- */

    nik_tutorial::BikeDetails *details = new nik_tutorial::BikeDetails;

    cout << "Enter the bike registration number: ";
    int id;
    cin >> id;
    cin.ignore(256, '\n');

    details->set_id(id);

    cout << "Enter the Manufacturer name: ";
    getline(cin, *details->mutable_name());

    cout << "Enter the Model : ";
    string model;
    getline(cin, model);
    if (!model.empty()) {
        details->set_model(model);
    }

    bike->set_allocated_details(details);

    /* --------------
        Type of Bike
       -------------- */

    cout << "Is this a Street, Cruiser or Racing Bike? ";
    string type;
    getline(cin, type);
    if (type == "Street") {
      bike->set_ridetype(nik_tutorial::RideType::STREET);
    } else if (type == "Cruiser") {
      bike->set_ridetype(nik_tutorial::RideType::CRUISER);
    } else if (type == "Racing") {
      bike->set_ridetype(nik_tutorial::RideType::RACING);
    } else {
      cout << "Unknown phone type.  Using default." << endl;
    }
  
    /* -----------------
        Insurance Number 
       ----------------- */
       
    nik_tutorial::InsuranceNumber* insurance = bike->add_insurances();
    
    while (true) {
    cout << "Enter an insurance number (or leave blank to finish): ";
    string number;
    getline(cin, number);

    insurance->set_number(number);

    if (number.empty()) {
      break;
    }
    
    }

    /*--------------
        TimeStamp
      ---------------  */

    *bike->mutable_last_updated() = TimeUtil::SecondsToTimestamp(time(NULL));
}

// Main function:  Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
    return -1;
  }

  nik_tutorial::WorkBook work_book;

  {
    // Read the existing work book.
    fstream input(argv[1], ios::in | ios::binary);
    if (!input) {
      cout << argv[1] << ": File not found.  Creating a new file." << endl;
    } else if (!work_book.ParseFromIstream(&input)) {
      cerr << "Failed to parse work book." << endl;
      return -1;
    }
  }

  // Add a bike
  PromptForBikeDetails(work_book.add_bikes());

  {
    // Write the new address book back to disk.
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!work_book.SerializeToOstream(&output)) {
      cerr << "Failed to write work book." << endl;
      return -1;
    }
  }

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
