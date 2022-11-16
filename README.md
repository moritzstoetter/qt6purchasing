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

This is a library wrapper, that provides an abstraction of the native app store libraries, and makes the necessary functionalities available in Qt6 and QML. Compatible with Apple App Store and Google Play Store.

Here's why:
* In-App-Purchasing might be an important way of monetizing your Qt/QML mobile app.
* QtPurchasing didn't make it to Qt6.


<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

This section should list any major frameworks/libraries used to bootstrap your project. Leave any add-ons/plugins for the acknowledgements section. Here are a few examples.

* [![Qt6][qt-badge]][https://www.qt.io/product/qt6]
* [![Objective-C][objc-badge]][Objective-C-url]
* [![Java][java-badge]][https://java.com]
* [![Cmake][cmake-badge]][https://cmake.com]

[qt-badge]: https://img.shields.io/badge/Qt6/QML-20232A?style=for-the-badge&logo=qt&logoColor=41cd52
[objc-badge]: https://img.shields.io/badge/Objective\C-20232A?style=for-the-badge&logo=apple&logoColor=gold
[java-badge]: https://img.shields.io/badge/Java-20232A?style=for-the-badge&logo=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgEAYAAAAj6qa3AAAEsGlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4KPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iWE1QIENvcmUgNS41LjAiPgogPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4KICA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIgogICAgeG1sbnM6ZXhpZj0iaHR0cDovL25zLmFkb2JlLmNvbS9leGlmLzEuMC8iCiAgICB4bWxuczp0aWZmPSJodHRwOi8vbnMuYWRvYmUuY29tL3RpZmYvMS4wLyIKICAgIHhtbG5zOnBob3Rvc2hvcD0iaHR0cDovL25zLmFkb2JlLmNvbS9waG90b3Nob3AvMS4wLyIKICAgIHhtbG5zOnhtcD0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wLyIKICAgIHhtbG5zOnhtcE1NPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvbW0vIgogICAgeG1sbnM6c3RFdnQ9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZUV2ZW50IyIKICAgZXhpZjpQaXhlbFhEaW1lbnNpb249IjMyIgogICBleGlmOlBpeGVsWURpbWVuc2lvbj0iMzIiCiAgIGV4aWY6Q29sb3JTcGFjZT0iMSIKICAgdGlmZjpJbWFnZVdpZHRoPSIzMiIKICAgdGlmZjpJbWFnZUxlbmd0aD0iMzIiCiAgIHRpZmY6UmVzb2x1dGlvblVuaXQ9IjIiCiAgIHRpZmY6WFJlc29sdXRpb249IjcyLzEiCiAgIHRpZmY6WVJlc29sdXRpb249IjcyLzEiCiAgIHBob3Rvc2hvcDpDb2xvck1vZGU9IjMiCiAgIHBob3Rvc2hvcDpJQ0NQcm9maWxlPSJzUkdCIElFQzYxOTY2LTIuMSIKICAgeG1wOk1vZGlmeURhdGU9IjIwMjItMTEt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
[cmake-badge]: https://img.shields.io/badge/Cmake-20232A?style=for-the-badge&logo=cmake&logoColor=white

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

To add In-App-Purchasing capabilities to your Qt6/QML project follow the steps below.

### Prerequisites

* Qt6/QML
* Apple StoreKit
* Android Billing Client

### Installation

1. Clone this repo into a folder in your project.
  ```sh
  git clone https://github.com/moritzstoetter/qt6purchasing.git
  ```
2. Move 'android/GooglePlayBilling.java' to 'QT_ANDROID_PACKAGE_SOURCE_DIR/src/com/COMPANY_NAME/APP_NAME/GooglePlayBilling.java'
  For more information on how to include custom Java-Code in your Android App see [Deploying an Application on Android](https://doc.qt.io/qt-6/deployment-android.html).
3. Add the qt6purchasing Library to your Project. In your projects 'CMakeLists.txt' add the following:
   ```sh
   target_link_libraries(APP_NAME
    PRIVATE
        ...
        store
    )
    ```
4. Expose the purchasing classes to QML. In your `main.cpp` include the following lines:
  ```sh
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
  ```sh
  import Store
  import Product  
  ```
2. Use it like this, for a product that is called "test_1" in the app store(s):
  ```sh
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
  'StoreItem.qml':
  ```sh
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
