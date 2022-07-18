import time

from django.http import JsonResponse

from rest_framework import views, permissions, status
from rest_framework.authentication import BasicAuthentication

from .gen import set_vehicle_detail, set_vehicle_detail_in_video, plate_numbers_in_video
from .gen_from_text_file import set_vehicle_detail_from_bulk_file, bulk_size
from .models import VehicleDetail
from .serializers import VehicleDetailSerializer

from knox.views import LoginView as KnoxLoginView


class LoginView(KnoxLoginView):
    authentication_classes = [BasicAuthentication]


class VehicleDetailView(views.APIView):
    # permission_classes = [permissions.IsAuthenticated]
    GET_KEY = 'license-plate-number'

    def get(self, request):
        params = request.GET
        # if self.GET_KEY not in params:
        #     return JsonResponse({'detail': f"'{self.GET_KEY}' should be given as param"},
        #                         status=status.HTTP_400_BAD_REQUEST)

        # plate_number = params[self.GET_KEY]
        # TODO: search for plate_number in database
        user = request.user
        auth = request.auth
        # vehicle_details = VehicleDetail.objects.all()
        # serializer = VehicleDetailSerializer(vehicle_details, many=True)

        # [Start] Create DB code
        VehicleDetail.objects.all().delete()

        print("start")
        start = time.time()

        f = open("vehicleDataForTheDemo.txt", 'r', encoding='Windows-1252')

        index = 0

        while True:
            bulk_list = []
            end_of_file = set_vehicle_detail_from_bulk_file(f, bulk_list)
            VehicleDetail.objects.bulk_create(bulk_list)

            index += 1
            print(index * bulk_size)

            if end_of_file:
                break

        f.close()

        print("end time :", time.time() - start)
        # [End] Create DB code

        # return JsonResponse(serializer.data, safe=False)
