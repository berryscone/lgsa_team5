from django.db import models


class VehicleDetail(models.Model):
    created = models.DateTimeField(auto_now_add=True)
    plate_number = models.CharField(max_length=10)
    owner = models.CharField(max_length=50)
    make = models.CharField(max_length=50)
    model = models.CharField(max_length=50)
