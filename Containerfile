# https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/9/html-single/building_running_and_managing_containers/index#proc_building-ubi-based-images_assembly_adding-software-to-a-ubi-container
FROM registry.access.redhat.com/ubi9/ubi
LABEL maintainer="Aleksandar Lazic"

RUN set -x \
  && id

USER root
RUN set -x \
  && dnf update --disablerepo=* --enablerepo=ubi-9-appstream-rpms --enablerepo=ubi-9-baseos-rpms -y \
  && rm -rf /var/cache/yum \
  && dnf install --disablerepo=* --enablerepo=ubi-9-appstream-rpms --enablerepo=ubi-9-baseos-rpms bzip2 \
  && dnf repolist

#CMD ["-D", "FOREGROUND"]
#ENTRYPOINT ["time-test"]

