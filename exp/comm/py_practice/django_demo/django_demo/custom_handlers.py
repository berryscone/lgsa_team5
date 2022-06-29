from django.http import JsonResponse

from rest_framework.views import exception_handler


def custom_exception_handler(exc, context):
    response = exception_handler(exc, context)
    if response is not None:
        return JsonResponse(response.data, status=response.status_code)
    return response
