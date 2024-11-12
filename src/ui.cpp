#include "../include/UI.h"

VariantManager::VariantManager(const char* vn, VariantManager* v)
    : builtVariant(nullptr) {
  feature_index = v ? v->feature_index : 0;
  airBagShow = v ? v->airBagShow : false;
  hasESC = v ? v->hasESC : false;
  hasParkingSensors = v ? v->hasParkingSensors : false;
  hasRearviewCam = v ? v->hasRearviewCam : false;
  hasABS = v ? v->hasABS : false;
  hasAirCondition = v ? v->hasAirCondition : true;
  hasPowerWindows = v ? v->hasPowerWindows : false;
  hasPowerLocks = v ? v->hasPowerLocks : false;
  hasKeylessEntry = v ? v->hasKeylessEntry : false;

  hasBluetoothConn = v ? v->hasBluetoothConn : false;
  hasUSBports = v ? v->hasUSBports : false;
  hasNavigationSys = v ? v->hasNavigationSys : false;
  hasAudioSystem = v ? v->hasAudioSystem : false;

  airConditionNum = v ? v->airConditionNum : "";
  airBagsNum = v ? v->airBagsNum : "";
  bodyMaterial = v ? v->bodyMaterial : "";
  groundClearanceNum = v ? v->groundClearanceNum : "";
  dimL = v ? v->dimL : "";
  dimB = v ? v->dimB : "";
  dimH = v ? v->dimH : "";
  variantName = vn;
  fuelSelect = v ? v->fuelSelect : 0;
  transmissionSelect = v ? v->transmissionSelect : 0;
  if (std::string(vn) == "Variant ") {
    variantName += std::to_string(variantCount);
  }
  variantCount++;

  //    airBagToggle = Toggle(&yesno, &airBagSelect, &airBagShow);
  input_airBagsNum = Input(&airBagsNum, "Number of Airbags");
  input_airBagsNum |= CatchEvent([&](Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });

  feature_tabs =
      Menu(&feature_entries, &feature_index, MenuOption::HorizontalAnimated());

  safetyFeatTabs = Container::Vertical({
      Checkbox("Airbags", &airBagShow) | size(WIDTH, LESS_THAN, 40),
      input_airBagsNum | size(WIDTH, EQUAL, 40) | Maybe(&airBagShow),
      Checkbox("Anti Lock Braking System", &hasABS) |
          size(WIDTH, LESS_THAN, 40),
      Checkbox("Electronic Stability Control", &hasESC) |
          size(WIDTH, LESS_THAN, 40),
      Checkbox("Read View Camera", &hasRearviewCam) |
          size(WIDTH, LESS_THAN, 40),
      Checkbox("Parking Sensors", &hasParkingSensors) |
          size(WIDTH, LESS_THAN, 40),
  });

  input_airCondition = Input(&airConditionNum, "Air Conditioning");
  input_airCondition |= CatchEvent([&](Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });

  comfortFeatTabs = Container::Vertical({
      Checkbox("Air Conditioning", &hasAirCondition) |
          size(WIDTH, LESS_THAN, 40),
      input_airCondition | size(WIDTH, EQUAL, 40) | Maybe(&hasAirCondition),
      Checkbox("Power Windows", &hasPowerWindows) | size(WIDTH, LESS_THAN, 40),
      Checkbox("Power Locks", &hasPowerLocks) | size(WIDTH, LESS_THAN, 40),
      Checkbox("Keyless Entry", &hasKeylessEntry) | size(WIDTH, LESS_THAN, 40),
  });

  technicalFeatTabs = Container::Vertical({
      Checkbox("Bluetooth Connection", &hasBluetoothConn) |
          size(WIDTH, LESS_THAN, 40),
      Checkbox("USB Ports", &hasUSBports) | size(WIDTH, LESS_THAN, 40),
      Checkbox("Navigation System", &hasNavigationSys) |
          size(WIDTH, LESS_THAN, 40),
      Checkbox("Audio System", &hasAudioSystem) | size(WIDTH, LESS_THAN, 40),
  });

  input_bodyMaterial = Input(&bodyMaterial, " Body Material ");

  input_groundClearance = Input(&groundClearanceNum, " Ground Clearance ");
  input_groundClearance |= CatchEvent([&](Event event) {
    if (event.is_character()) {
      char ch = event.character()[0];

      if (std::isdigit(ch)) {
        return false;
      } else if (ch == '.') {
        if (groundClearanceNum.find('.') != std::string::npos) {
          return true;
        }
        return false;
      }
      return true;
    }
    return false;
  });

  input_dimensions[0] = Input(&dimL, " Length");
  input_dimensions[0] |= CatchEvent([&](Event event) {
    if (event.is_character()) {
      char ch = event.character()[0];

      if (std::isdigit(ch)) {
        return false;
      } else if (ch == '.') {
        if (dimL.find('.') != std::string::npos) {
          return true;
        }
        return false;
      }
      return true;
    }
    return false;
  });

  input_dimensions[1] = Input(&dimB, " Width");
  input_dimensions[1] |= CatchEvent([&](Event event) {
    if (event.is_character()) {
      char ch = event.character()[0];

      if (std::isdigit(ch)) {
        return false;
      } else if (ch == '.') {
        if (dimB.find('.') != std::string::npos) {
          return true;
        }
        return false;
      }
      return true;
    }
    return false;
  });

  input_dimensions[2] = Input(&dimH, " Height");
  input_dimensions[2] |= CatchEvent([&](Event event) {
    if (event.is_character()) {
      char ch = event.character()[0];

      if (std::isdigit(ch)) {
        return false;
      } else if (ch == '.') {
        if (dimH.find('.') != std::string::npos) {
          return true;
        }
        return false;
      }
      return true;
    }
    return false;
  });

  buildFeatComp = Container::Vertical({
      input_bodyMaterial,
      input_groundClearance | size(WIDTH, EQUAL, 10),
      input_dimensions[0] | size(WIDTH, EQUAL, 10),
      input_dimensions[1] | size(WIDTH, EQUAL, 10),
      input_dimensions[2] | size(WIDTH, EQUAL, 10),
  });

  buildFeatTabs = Renderer(buildFeatComp, [&] {
    return vbox({
        hbox(text("Body Material        : "),
             input_bodyMaterial->Render() | size(WIDTH, EQUAL, 30)),
        hbox(text("Ground Clearance     : "),
             input_groundClearance->Render() | size(WIDTH, EQUAL, 30)),
        hbox(text("Dimensions (in m)    : "),
             input_dimensions[0]->Render() | size(WIDTH, EQUAL, 30)),
        hbox(text("                       "),
             input_dimensions[1]->Render() | size(WIDTH, EQUAL, 30)),
        hbox(text("                       "),
             input_dimensions[2]->Render() | size(WIDTH, EQUAL, 30)),
    });
  });

  input_variantName = Input(&variantName, " Variant Name ");
  input_additionalFeat = Input(&additionalFeat, " Additional Features ");

  dropdown_fueltype = Dropdown(&fueltype, &fuelSelect);
  dropdown_transmission = Dropdown(&transmission, &transmissionSelect);

  input_price = Input(&price, " Price ");
  input_price |= CatchEvent([&](Event event) {
    if (event.is_character()) {
      char ch = event.character()[0];
      if (std::isdigit(ch)) {
        return false;
      } else if (ch == '.') {
        if (price.find('.') != std::string::npos) {
          return true;
        }
        return false;
      }
      return true;
    }
    return false;
  });

  basicInfoComp =
      Container::Vertical({input_variantName, input_price, dropdown_fueltype,
                           dropdown_transmission, input_additionalFeat});

  basicInfo = Renderer(basicInfoComp, [&] {
    return vbox({
        hbox(text("Variant Name           : "),
             input_variantName->Render() | size(WIDTH, EQUAL, 30)),
        hbox(text("Variant Price          : "),
             input_price->Render() | size(WIDTH, EQUAL, 30)),
        hbox(text("Fuel Type              : "),
             dropdown_fueltype->Render() | size(WIDTH, EQUAL, 30)),
        hbox(text("Car Transmission       : "),
             dropdown_transmission->Render() | size(WIDTH, EQUAL, 30)),
        hbox(text("Additional Features    : "),
             input_additionalFeat->Render() | size(WIDTH, EQUAL, 30)),
    });
  });

  tab_content = Container::Tab(
      {safetyFeatTabs, comfortFeatTabs, technicalFeatTabs, buildFeatTabs},
      &feature_index);

  main_container = Container::Vertical({
      basicInfo,
      Container::Horizontal({feature_tabs}),
      tab_content,
  });

  // Create the main vertical container for the variants
  renderer = Renderer(main_container, [&] {
    return vbox({
        hbox({basicInfo->Render()}),
        hbox({text(" ")}),
        window(text("Features"), vbox({hbox({
                                           feature_tabs->Render(),
                                       }) | hcenter,
                                       tab_content->Render()})) |
            size(WIDTH, EQUAL, 78) | size(HEIGHT, EQUAL, 15) | hcenter,
    });
  });
}

Component VariantManager::getComponent() { return renderer; }

std::string VariantManager::getVarName() { return this->variantName; }

bool VariantManager::build() {
  SafetyFeatures sf(airBagShow, hasABS, hasESC, hasRearviewCam,
                    hasParkingSensors, utils::stoi(airBagsNum));
  ComfortFeatures cf(hasAirCondition, hasPowerWindows, hasPowerLocks,
                     hasKeylessEntry, utils::stoi(airConditionNum));
  TechnoFeatures tf(hasBluetoothConn, hasUSBports, hasNavigationSys,
                    hasAudioSystem);

  double dim[3];
  dim[0] = utils::stod(dimL);
  dim[1] = utils::stod(dimB);
  dim[2] = utils::stod(dimH);
  BuildFeatures bf(bodyMaterial, utils::stod(groundClearanceNum), dim);
  if (!bf.isValid()) return false;

  CarVariant* v = new CarVariant(variantName, utils::stod(price));
  v->setFuelType(fueltypeMap[fuelSelect])
      .setCarTransmission(transmissionMap[transmissionSelect])
      .pushAdditionalFeatures(additionalFeat)
      .setSafetyFeatures(sf)
      .setComfortFeatures(cf)
      .setTechnoFeatures(tf)
      .setBuildFeatures(bf);

  if (!v->isValid()) {
    delete v;
    return false;
  }

  builtVariant = v;

  return true;
}

CarVariant& VariantManager::getVariant() { return *builtVariant; }

VariantManager::~VariantManager() { variantCount--; }

std::vector<std::string> VariantManager::feature_entries = {
    " Safety Features ",
    " Comfort Features ",
    " Technical Features ",
    " Build Features ",
};

std::vector<std::string> VariantManager::yesno = {"Yes", "No"};
int VariantManager::variantCount = 1;
std::vector<std::string> VariantManager::fueltype = {"PETROL", "DIESEL", "CNG",
                                                   "BIFUEL", "ELECTRIC"};
std::vector<Fuel> VariantManager::fueltypeMap = {
    Fuel::PETROL, Fuel::DIESEL, Fuel::CNG, Fuel::BIFUEL, Fuel::ELECTRIC};
std::vector<std::string> VariantManager::transmission = {
    "MANUAL", "SEMIAUTOMATIC", "AUTOMATIC"};
std::vector<CarTransmission> VariantManager::transmissionMap = {
    CarTransmission::MANUAL, CarTransmission::SEMIAUTOMATIC,
    CarTransmission::AUTOMATIC};