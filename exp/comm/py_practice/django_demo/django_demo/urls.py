from django.urls import path, include

from knox import views as knox_views

from demo_app.views import LoginView

urlpatterns = [
    path('', include('demo_app.urls')),
    path('login/', LoginView.as_view(), name='knox_login'),
    path('logout/', knox_views.LogoutView.as_view(), name='knox_logout'),
    path('logoutall/', knox_views.LogoutAllView.as_view(), name='knox_logoutall'),
]
