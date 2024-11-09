#include "../include/UI.h"

void employeeProfile(std::vector<NewCar>& inventoryNewCar) {
  while (1) {
    int option = mainMenuEmployee();

    switch (option) {
      case 0: {
        NewCar* aCar = regCar();
        if (aCar == nullptr) break;

        if (NewCar::storeIntoDB(aCar))
          std::cout << "Stored in db\n";
        else
          std::cout << "Newly Reg Car was not stored in db\n";

        inventoryNewCar.push_back(*aCar);
        break;
      }

      case 1:
        // for (auto item : inventoryNewCar) {
        //   item.display();
        // }

        // searchCars(inventoryNewCar);
        break;

      default:
        return;
        break;
    }
    // std::cin.clear();
    // fflush(stdin);
    // char c = getchar();
  }
}

void customerProfile(std::vector<NewCar>& inventoryNewCar) {
  while (1) {
    int option = mainMenuCustomer();

    switch (option) {
      case 0:
        for (auto item : inventoryNewCar) {
          item.display();
        }

        searchCars(inventoryNewCar);
        break;

      default:
        return;
        break;
    }
    // std::cin.clear();
    // fflush(stdin);
    // char c = getchar();
  }
}