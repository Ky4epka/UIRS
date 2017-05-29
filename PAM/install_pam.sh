#/bin/bash
cp libsec_priv_caps_auth.so /lib/x86_64-linux-gnu/security/libsec_priv_caps_auth.so
cp libsec_priv_caps_auth.so ./PAM_Build/lib/security/libsec_priv_caps_auth.so
cp libsec_priv_caps_auth.so ./PAM_Build/lib/x86_64-linux-gnu/security/libsec_priv_caps_auth.so
cp testload.run /lib/x86_64-linux-gnu/security/testload.run

cp /home/ky4epok/UIRS/PAM/PAM_Build/PAM_configs/etc/pam.d/sec_priv_caps_auth /etc/pam.d/sec_priv_caps_auth
cp /home/ky4epok/UIRS/PAM/PAM_Build/PAM_configs/usr/share/pam-configs/sec_priv_caps_auth /usr/share/pam-configs/sec_priv_caps_auth

mkdir /usr/lib/_sec_priv
mkdir /usr/lib/_sec_priv/libsec_priv_locallook
cp settings.cfg /usr/lib/_sec_priv/settings.cfg
cp local.db /usr/lib/_sec_priv/local.db
cp libsec_priv_look.so /usr/lib/_sec_priv/libsec_priv_look.so
cp libsec_priv_locallook/libsec_priv_locallook.so /usr/lib/_sec_priv/libsec_priv_locallook/libsec_priv_locallook.so
cp libsec_priv_locallook/module.cfg /usr/lib/_sec_priv/libsec_priv_locallook/module.cfg
