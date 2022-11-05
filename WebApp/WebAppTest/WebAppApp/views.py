from django.shortcuts import render

# Create your views here.
def home(request):
    return render(request, 'index.html')

def browse(request):
    return render(request, 'browse.html')