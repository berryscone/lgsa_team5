from django.db import models


class VehicleDetail(models.Model):
    created = models.DateTimeField(auto_now_add=True)
    plate_number = models.CharField(db_index=True, max_length=10, default='')
    status = models.CharField(max_length=50, default='')
    reg_exp = models.CharField(max_length=50, default='')
    owner = models.CharField(max_length=50, default='')
    birth = models.CharField(max_length=50, default='')
    address = models.CharField(max_length=100, default='')
    year = models.CharField(max_length=50, default='')
    make = models.CharField(max_length=50, default='')
    model = models.CharField(max_length=50, default='')
    color = models.CharField(max_length=50, default='')

