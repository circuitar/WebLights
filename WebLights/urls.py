from django.conf.urls import patterns, include, url

from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    url(r'^$', 'WebLightsPanel.views.home', name='home'),
    url(r'^set/$', 'WebLightsPanel.views.set_device', name='set'),
    url(r'^get/(?P<device>[a-zA-Z0-9-]+)/$', 'WebLightsPanel.views.get_device', name='get'),
    url(r'^admin/', include(admin.site.urls)),
)
