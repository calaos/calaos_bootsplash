#!/bin/sh
export CODESIGN_ALLOCATE=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/codesign_allocate
if [ "${PLATFORM_NAME}" == "iphoneos" ]; then
	$HOME/gen_entitlements.py "com.calaos.${PROJECT_NAME}" "${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}/${PROJECT_NAME}.xcent";
	codesign -f -s "iPhone developer" --resource-rules "${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}/ResourceRules.plist" \
		 --entitlements "${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}/${PROJECT_NAME}.xcent"  "${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}/"
fi
