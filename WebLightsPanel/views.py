from django.shortcuts import render
from django.http import HttpResponse, HttpResponseForbidden, HttpResponseNotFound
from models import Device
from django.views.decorators.csrf import csrf_exempt


def home(request):
    return render(request, 'index.html', {
        'devices': Device.objects.all()
    })


@csrf_exempt
def set_device(request):
    if request.method == 'POST':
        # Check if the device code exists and store the value in the database
        for device, value in request.POST.items():
            try:
                device = Device.objects.get(code=device)
                device.value = float(value)
                device.save()
            except (Device.DoesNotExist, ValueError):
                pass

        return HttpResponse()

    return HttpResponseForbidden()


def get_device(request, device):
    if request.method == 'GET':
        try:
            device = Device.objects.get(code=device)
            return HttpResponse(int(device.value))
        except Device.DoesNotExist:
            return HttpResponseNotFound()

    return HttpResponseForbidden()
