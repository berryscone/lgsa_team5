from django.http import JsonResponse

from rest_framework import views, permissions, status
from rest_framework.authentication import BasicAuthentication

from .models import VehicleDetail
from .serializers import VehicleDetailSerializer

from knox.views import LoginView as KnoxLoginView


class LoginView(KnoxLoginView):
    authentication_classes = [BasicAuthentication]


class VehicleDetailView(views.APIView):
    permission_classes = [permissions.IsAuthenticated]
    GET_KEY = 'license-plate-number'

    def get(self, request):
        params = request.GET
        if self.GET_KEY not in params:
            return JsonResponse({'detail': f"'{self.GET_KEY}' should be given as param"},
                                status=status.HTTP_400_BAD_REQUEST)

        # plate_number = params[self.GET_KEY]
        # TODO: search for plate_number in database
        user = request.user
        auth = request.auth
        vehicle_details = VehicleDetail.objects.all()
        serializer = VehicleDetailSerializer(vehicle_details, many=True)
        return JsonResponse(serializer.data, safe=False)
