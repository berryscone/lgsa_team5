# Generated by Django 4.0.5 on 2022-06-29 05:05

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='VehicleDetail',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('created', models.DateTimeField(auto_now_add=True)),
                ('plate_number', models.CharField(max_length=10)),
                ('owner', models.CharField(max_length=50)),
                ('make', models.CharField(max_length=50)),
                ('model', models.CharField(max_length=50)),
            ],
        ),
    ]