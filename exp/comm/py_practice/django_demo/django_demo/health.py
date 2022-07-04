from health_check.views import MainView

from rest_framework.views import APIView
from rest_framework import permissions


class HealthCheckCustomView(MainView, APIView):
    permission_classes = [permissions.IsAuthenticated]
