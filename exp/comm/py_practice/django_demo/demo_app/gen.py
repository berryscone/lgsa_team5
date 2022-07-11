from faker import Faker
from faker_vehicle import VehicleProvider
from datetime import datetime

fake = Faker('en_US')
fake.add_provider(VehicleProvider)
Faker.seed(0)

plate_numbers_in_video = list(set([
    "LKY1360",
    "HHF6697",
    "GVP9164",
    "LBX9051",
    "06062",
    "LBV6157",
    "LVH6056",
    "ZPV5837",
    "ZDE1985",
    "JBK6142",
    "ZRP9348",
    "MSJWHY",
    "FORNER2",
    "LVM6107",
    "KKM1789",
    "KFD6960",
    "ZLD0922",
    "KYT3950",
    "LKS9443",
    "YDS5255",
    "KGJ8487",
    "ZNS2724",
    "ZNM2197",
    "ZGS7240",
    "LPH0511",
    "KYG9827",
    "YAM0025",
    "FJC656#",
    "CPTMGN2",
    "HYY8868",
    "JHG7802",
    "BETELU",
    "LBX9129",
    "LDX1620",
    "KCS6722",
    "LLX9680",
    "KYR3878",
    "JJC5503",
    "LMC5535",
    "JXK1447",
    "8210076",
    "KXZ7041",
    "KZS9188",
    "LMT7276",
    "K66319K",
    "LNF6519",
    "LDD4877",
    "HMZ2628",
    "JDR7141",
    "LWY0004",
    "LHS8870",
    "HHF6697",
    "LBV6157",
    "LVH6056",
    "KTC0964",
    "LVB2188",
    "FORNER2",
    "LVM6107",
    "KKM1789",
    "KFD6960",
    "LRK8742",
    "KYT3950",
    "LJF8922",
    "LKS9443",
    "KCC9291",
    "HFZ9114",
    "ZNW2197",
    "GXV3315",
    "ZVD6300",
    "LYB2009",
    "YAM0025",
    "YZR0143",
    "CAR3963",
    "GDC4098",
    "GYB9496",
    "GRN0422",
    "LST6350",
    "KNM0465",
    "LST8113",
    "KYRO",
    "LKP5331",
    "KTC8699",
    "LXZ0242",
    "LCL6497",
    "KYD4352",
    "KPA8727",
    "KZT4724",
    "HLF0755",
    "LLS9557",
    "##9479",
    "JBV7597",
    "LDP9038",
    "LXE5831",
    "KDX9805",
    "HPM2638"
]))


def set_vehicle_detail_in_video(i, vehicle):
    if i < len(plate_numbers_in_video):
        plate = plate_numbers_in_video[i]
        if i < 10:
            random_num_for_status = 2
        elif i < 20:
            random_num_for_status = 5
        elif i < 30:
            random_num_for_status = 8
        else:
            random_num_for_status = 10
    else:
        plate = fake.bothify('???####', letters='ABCDEFGHJKLMNPRSTVWXYZ')
        random_num_for_status = fake.pyint(1, 100)
    set_vehicle_detail_more(plate, random_num_for_status, vehicle)


def set_vehicle_detail(vehicle):
    plate = fake.bothify('???####', letters='ABCDEFGHJKLMNPRSTVWXYZ')
    random_num_for_status = fake.pyint(1, 100)
    set_vehicle_detail_more(plate, random_num_for_status, vehicle)


def set_vehicle_detail_more(plate, random_num_for_status, vehicle):
    vehicle.plate_number = plate
    if random_num_for_status < 3:
        status = "Owner Wanted"
    elif random_num_for_status < 6:
        status = "Unpaid Fines - Tow"
    elif random_num_for_status < 9:
        status = "Stolen"
    else:
        status = "No Wants / Warrants"
    vehicle.status = status
    vehicle.reg_exp = fake.date_between_dates(date_start=datetime(2022, 1, 1),
                                              date_end=datetime(2024, 5, 1)).strftime("%m/%d/%Y")
    vehicle.owner = fake.name()
    vehicle.birth = fake.date_of_birth().strftime("%m/%d/%Y")
    vehicle.address = fake.address()
    vehicle.year = fake.vehicle_year()
    vehicle.make = fake.vehicle_make()
    vehicle.model = fake.vehicle_model()
    vehicle.color = fake.safe_color_name()
