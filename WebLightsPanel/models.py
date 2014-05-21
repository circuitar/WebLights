from django.db import models


class Device(models.Model):
    name = models.CharField(max_length=100)
    code = models.SlugField(max_length=50)
    min_value = models.FloatField()
    max_value = models.FloatField()
    units = models.CharField(max_length=100)
    value = models.FloatField()
    description = models.TextField(blank=True)

    class Meta:
        verbose_name = 'Device'
        verbose_name_plural = 'Devices'
        ordering = ['name']

    def __unicode__(self):
        return self.name
