import time

from django.http import JsonResponse

from rest_framework import views, permissions, status
from rest_framework.authentication import BasicAuthentication

from .gen import set_vehicle_detail, set_vehicle_detail_in_video, plate_numbers_in_video
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

        for i in range(len(plate_numbers_in_video)):
            vehicle = VehicleDetail()
            set_vehicle_detail_in_video(i, vehicle)
            vehicle.save()

        bulk_size = 10000

        for i in range(2500):
            bulk_list = []
            for j in range(bulk_size):
                vehicle = VehicleDetail()
                set_vehicle_detail(vehicle)
                bulk_list.append(vehicle)
            VehicleDetail.objects.bulk_create(bulk_list)
            print((i + 1) * bulk_size)

        print("end time :", time.time() - start)
        # [End] Create DB code

        # return JsonResponse(serializer.data, safe=False)
