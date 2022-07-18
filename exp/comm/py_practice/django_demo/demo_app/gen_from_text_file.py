from demo_app.models import VehicleDetail

bulk_size = 10000
pre_read_plate_num = ''


def set_vehicle_detail_from_bulk_file(f, bulk_list):
    end_of_file = False

    for i in range(bulk_size):
        vehicle = VehicleDetail()
        end_of_file = set_vehicle_detail(f, vehicle)
        bulk_list.append(vehicle)
        if end_of_file:
            break

    return end_of_file


def set_vehicle_detail(f, vehicle):
    set_plate_number(f, vehicle)

    vehicle.status = read_line(f)
    vehicle.reg_exp = read_line(f)
    vehicle.owner = read_line(f)
    vehicle.birth = read_line(f)
    vehicle.address = read_line(f) + ' ' + read_line(f)
    vehicle.year = read_line(f)
    vehicle.make = read_line(f)
    vehicle.model = read_line(f)

    end_of_file = set_color_and_pre_read_plate_num(f, vehicle)

    return end_of_file


def read_line(file):
    return file.readline().strip()


def set_plate_number(f, vehicle):
    global pre_read_plate_num

    if not pre_read_plate_num:
        vehicle.plate_number = read_line(f)
    else:
        vehicle.plate_number = pre_read_plate_num


def set_color_and_pre_read_plate_num(f, vehicle):
    global pre_read_plate_num

    token_color_and_plate_num = read_line(f).split(sep='$')

    vehicle.color = token_color_and_plate_num[0]

    if token_color_and_plate_num[1]:
        pre_read_plate_num = token_color_and_plate_num[1]

    end_of_file = not token_color_and_plate_num[1]

    return end_of_file
