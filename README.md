# qt6purchasing
<a name="readme-top"></a>


<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/moritzstoetter/qt6purchasing">
    <img src=" logo.png" alt="Logo" width="500" height="auto">
  </a>

  <h3 align="center">Qt6/QML In-App-Purchasing</h3>

  <p align="center">
    Bringing In-App-Purchasing to Qt6
    <br />
    <br />
    <a href="https://github.com/moritzstoetter/qt6purchasing/issues">Report Bug</a>
    ·
    <a href="https://github.com/moritzstoetter/qt6purchasing/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This project provides a library wrapper, that provides an abstraction of the native app store libraries, and makes the necessary functionalities available in Qt6 and QML. Compatible with Apple App Store and Google Play Store.

Here's why:
* In-App-Purchasing might be an important way of monetizing your Qt/QML mobile app.
* QtPurchasing didn't make it to Qt6.


<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

[![Qt6][qt-badge]][qt-url]<br>
[![Cpp][cpp-badge]][cpp-url]<br>
[![Objective-C][objc-badge]][objc-url]<br>
[![Java][java-badge]][java-url]<br>
[![Cmake][cmake-badge]][cmake-url]<br>

[qt-badge]: https://img.shields.io/badge/Qt/QML-6-20232A?style=for-the-badge&logo=qt&logoColor=41cd52
[qt-url]: https://www.qt.io/product/qt6
[cpp-badge]: https://img.shields.io/badge/C++-17-20232A?style=for-the-badge&logo=cplusplus&logoColor=blue
[cpp-url]: https://isocpp.org/
[objc-badge]: https://img.shields.io/badge/Objective--C-20232A?style=for-the-badge&logo=apple&logoColor=gold
[objc-url]: https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/ProgrammingWithObjectiveC/Introduction/Introduction.html
[java-badge]: https://img.shields.io/badge/Java-20232A?style=for-the-badge
[java-url]: https://java.com
[cmake-badge]: https://img.shields.io/badge/Cmake-20232A?style=for-the-badge&logo=cmake&logoColor=white
[cmake-url]: https://cmake.org

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

To add In-App-Purchasing capabilities to your Qt6/QML project follow the steps below.

### Prerequisites

* Qt/QML 6
* Apple StoreKit
* Android Billing Client

### Installation

1. Clone this repo into a folder in your project.
  ```sh
  git clone https://github.com/moritzstoetter/qt6purchasing.git
  ```
2. Move 'android/GooglePlayBilling.java' to `QT_ANDROID_PACKAGE_SOURCE_DIR/src/com/COMPANY_NAME/APP_NAME/GooglePlayBilling.java`
  For more information on how to include custom Java-Code in your Android App see [Deploying an Application on Android](https://doc.qt.io/qt-6/deployment-android.html).
3. Add the qt6purchasing Library to your Project. In your project's `CMakeLists.txt` add the following:
   ```cmake
   target_link_libraries(APP_NAME
    PRIVATE
        ...
        store
    )
    ```
4. Expose the purchasing classes to QML. In your `main.cpp` include the following lines:
  ```cpp
  #if defined Q_OS_ANDROID
  #include <QJniObject>
  #include <QCoreApplication>
  #include "backend/store/android/googleplaystorebackend.h"
  #include "backend/store/android/googleplaystoreproduct.h"
  #include "backend/store/android/googleplaystoretransaction.h"
  #endif

  #if defined Q_OS_DARWIN
  #include "backend/store/apple/appleappstorebackend.h"
  #include "backend/store/apple/appleappstoreproduct.h"
  #include "backend/store/apple/appleappstoretransaction.h"
  #endif

  ...

  qmlRegisterUncreatableType<AbstractStoreBackend>("AbstractStoreBackend",1,0,"AbstractStoreBackend","AbstractProduct uncreatable");
  qmlRegisterUncreatableType<AbstractProduct>("AbstractProduct",1,0,"AbstractProduct","AbstractProduct uncreatable");
  qmlRegisterUncreatableType<AbstractTransaction>("AbstractTransaction",1,0,"AbstractTransaction","AbstractTransaction uncreatable");
#if defined Q_OS_ANDROID
  qmlRegisterType<GooglePlayStoreBackend>("Store", 1, 0, "Store");
  qmlRegisterType<GooglePlayStoreProduct>("Product", 1, 0, "Product");
#elif defined Q_OS_DARWIN
  qmlRegisterType<AppleAppStoreBackend>("Store", 1, 0, "Store");
  qmlRegisterType<AppleAppStoreProduct>("Product", 1, 0, "Product");
#endif
  ```


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

1. In your QML file include the purchasing classes:
  ```qml
  import Store
  import Product  
  ```
2. Use it like this, for a product that is called "test_1" in the app store(s):
  ```qml
  Store {
    id: iapStore
    Product {
      id: testingProduct
      identifier: "test_1"
      type: Product.Consumable
    }
  }

  StoreItem {
    id: testingStoreItem
    product: testingProduct

    onIapCompleted: root.accepted()
  }
  ```
  `StoreItem.qml`:
  ```qml
  import QtQuick
  import Product

  Item {
    id: root
    required property Product product

    signal iapCompleted

    enum PurchasingStatus {
        NoPurchase,
        PurchaseProcessing,
        PurchaseSuccess,
        PurchaseFail
    }
    property int purchasingStatus: StoreItem.PurchasingStatus.NoPurchase

    function purchase() {
        purchasingStatus = StoreItem.PurchasingStatus.PurchaseProcessing
        product.purchase()
    }

    function finalize(transaction) {
        purchasingStatus = StoreItem.PurchasingStatus.PurchaseSuccess
        transaction.finalize()
      restoreFailedTimerId.stop();
    }

    function restore() {
      if(!restoreFailedTimerId.running) {
          purchasingStatus = StoreItem.PurchasingStatus.PurchaseProcessing
          restoreFailedTimerId.start()
          product.restore()
      }
    }

  Timer {
      id: restoreFailedTimerId
      interval: 10000
      onTriggered: {
          purchasingStatus = StoreItem.PurchasingStatus.PurchaseFail
      }
  }

    Connections {
        target: product
        function onPurchaseSucceeded(transaction) {
            finalize(transaction)
        }
        function onPurchaseRestored(transaction) {
            finalize(transaction)
        }
        function onPurchaseFailed(transaction) {
            purchasingStatus = StoreItem.PurchasingStatus.PurchaseFail
        }
        function onPurchaseConsumed(transaction) {
            root.iapCompleted()
        }
    }
  }
  ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTRIBUTING -->
## Contributing

Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Moritz Stötter - [moritzstoetter.dev](https://www.moritzstoetter.dev) - hi@moritzstoetter.dev


<p align="right">(<a href="#readme-top">back to top</a>)</p>
