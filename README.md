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
[java-badge]: https://img.shields.io/badge/Java-20232A?style=for-the-badge&logo=data%3Aimage%2Fpng%3Bbase64%2CiVBORw0KGgoAAAANSUhEUgAAACAAAAAgEAYAAAAj6qa3AAAEsGlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4KPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iWE1QIENvcmUgNS41LjAiPgogPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4KICA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIgogICAgeG1sbnM6ZXhpZj0iaHR0cDovL25zLmFkb2JlLmNvbS9leGlmLzEuMC8iCiAgICB4bWxuczp0aWZmPSJodHRwOi8vbnMuYWRvYmUuY29tL3RpZmYvMS4wLyIKICAgIHhtbG5zOnBob3Rvc2hvcD0iaHR0cDovL25zLmFkb2JlLmNvbS9waG90b3Nob3AvMS4wLyIKICAgIHhtbG5zOnhtcD0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wLyIKICAgIHhtbG5zOnhtcE1NPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvbW0vIgogICAgeG1sbnM6c3RFdnQ9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZUV2ZW50IyIKICAgZXhpZjpQaXhlbFhEaW1lbnNpb249IjMyIgogICBleGlmOlBpeGVsWURpbWVuc2lvbj0iMzIiCiAgIGV4aWY6Q29sb3JTcGFjZT0iMSIKICAgdGlmZjpJbWFnZVdpZHRoPSIzMiIKICAgdGlmZjpJbWFnZUxlbmd0aD0iMzIiCiAgIHRpZmY6UmVzb2x1dGlvblVuaXQ9IjIiCiAgIHRpZmY6WFJlc29sdXRpb249IjcyLzEiCiAgIHRpZmY6WVJlc29sdXRpb249IjcyLzEiCiAgIHBob3Rvc2hvcDpDb2xvck1vZGU9IjMiCiAgIHBob3Rvc2hvcDpJQ0NQcm9maWxlPSJzUkdCIElFQzYxOTY2LTIuMSIKICAgeG1wOk1vZGlmeURhdGU9IjIwMjItMTEtMTZUMTU6MDQ6MjUrMDE6MDAiCiAgIHhtcDpNZXRhZGF0YURhdGU9IjIwMjItMTEtMTZUMTU6MDQ6MjUrMDE6MDAiPgogICA8eG1wTU06SGlzdG9yeT4KICAgIDxyZGY6U2VxPgogICAgIDxyZGY6bGkKICAgICAgc3RFdnQ6YWN0aW9uPSJwcm9kdWNlZCIKICAgICAgc3RFdnQ6c29mdHdhcmVBZ2VudD0iQWZmaW5pdHkgUGhvdG8gMS4xMC41IgogICAgICBzdEV2dDp3aGVuPSIyMDIyLTExLTE2VDE1OjA0OjI1KzAxOjAwIi8%2BCiAgICA8L3JkZjpTZXE%2BCiAgIDwveG1wTU06SGlzdG9yeT4KICA8L3JkZjpEZXNjcmlwdGlvbj4KIDwvcmRmOlJERj4KPC94OnhtcG1ldGE%2BCjw%2FeHBhY2tldCBlbmQ9InIiPz4KlQ1YAAABgWlDQ1BzUkdCIElFQzYxOTY2LTIuMQAAKJF1kd8rg1EYxz%2FbiJgoLnbhYglXGzO1uFG2hJLWTBlutne%2F1H68ve%2BWllvldkWJG78u%2BAu4Va6VIlJyKdfEDXo9r6kt2XN6zvM533Oep3OeA9ZwRsnqDR7I5gpaaNLvXIgsOpueaKWDNgZxRBVdHQ8GZ6hr77dYzHjtNmvVP%2FevtcYTugKWZuExRdUKwlPCM6sF1eQt4S4lHY0Lnwi7NLmg8I2pxyr8bHKqwp8ma%2BFQAKwdws5UDcdqWElrWWF5Ob3ZTFH5vY%2F5EnsiNz8nsUe8G50Qk%2FhxMs0EAXwMMSqzDzdeBmRFnXzPT%2F4seclVZFYpobFCijQFXKIWpXpCYlL0hIwMJbP%2Ff%2FuqJ4e9lep2PzQ%2BGsZrHzRtwlfZMD4ODOPrEGwPcJ6r5uf3YeRN9HJV692D9nU4vahqsW042wDHvRrVoj%2BSTdyaTMLLMbRFoPMKWpYqPfvd5%2BgOwmvyVZewswv9cr59%2BRtsL2fow%2BuoUQAAAAlwSFlzAAALEwAACxMBAJqcGAAABSlJREFUaN7lmHtMlXUYx8%2BF20EQRM4xwIMooIGClxSyIEiKAhsgeQCnQgSmYiKoJdeQcqmkqJRoObOs0NRVajo3Wzan0iqNWl7anOnWumdYdm%2BzP76f%2FjjMxK1ADvDPZ4f3fc95f8%2Fv%2BX2f7%2FMYDD3s74%2B0pLKkMvd5ov9p0Zxo6Ct%2FWrAxX8wbJtrC%2B1gAvGrFx8mAIVv6WABiE8Umf3FwYh9ZuCVPrF5EBiSIPmd6%2BcLNlWJ2qXjsKzExp5cvvL9DzLeLJ78W508QPW5HFF8UAxrJiFH838PFFmxIEyPmiiuniRcaxIos0e%2BSOPQH8b7%2BYvoxMZzrZndXKWte4oRZ4q73xdMfiAU7KHeXRUeduKqQ60Wi9RlX23F2bhQ7d%2BBz8eOlYjI77v2zWO4p7iMAk%2FeJnhtc9YyPFtdwti9uE7M4%2B6Z6cdJzHAX4IDtvupNAuqrI2UvE4zXizk%2FEkEbn%2B3b%2FIrYNF2OsvUTlb3pXPMhONu0mM%2Bqd72tdQAbMIEM4Csbprm5pKWfl08Wt23F2R5zvWz5C%2FJMAbZ9NBrW5eAAMcWJYudj8hpiG4THFiCMqxFOviJc2isV%2BdIdFrt7dsbCYhyhv4bS7X1LPA8RcymX7CrFxk%2Bhb5mo7P8DZwRktztejKY8z07n%2FNjFyj7j%2FLaoBmWM%2B1%2BH7LXxvO47xEcolgTQe6K6dTcK5%2FYjD%2B5D29RDMwbpeufrzIYidOUOMihTnzBeDbP8S4LcR2d8RTXzGwmgxg4yzbOwqcbuD%2Bk1dX%2FuE2EC5S6eLC0bcDEc6CSTNUFAxgWQOYD9OezwWJ%2Fk6VeQvNOS86Mb3JJN551vEqQe7KAD9QklRIn%2F4JXFPCmJ3kTO8DJVfLD7pI644xXWeW2Pk%2F5%2BKtQS41Fuctl6MwyD5rCOwpg6ZgbE6wRFq3tZVqX8r4sSOD%2BGFovg8mjM6LlCM3y9OLGAhH4ljqPsxuWgDA5DwTDJoA0cM62yaeO33Cr8ifsfRyG3rZvELhKvp1rzc2ZmR8Bb4z9Hw7kRM0QjD3E5%2Bt5%2B4t5mMC%2Ba5o90cgLyHxc%2BixB1VBASRrCPFlxCYJVSDpfiDppN0i6j5e9zf4otGnCMT6CIjEb3KrYghVcV0%2BUbV%2B9cQq2fFeSzsKbRg85viy0chBmgTmtFQjXOk%2Fc1iXjCc68a9dI8MSqLJKH%2BeMwT2EB%2Fgh9FJxeObp%2FCC41kIATCGXf15t5kQVTeMczEjNAUDczYeb3%2BCTLCJVdy3mDNee7e4fiBnGUfYcoay6tvTnB6W1o8yFoORGUAV8NoizqJOHwoRv18p%2FkaK%2F%2FoNnylb7cwNWn9iKrwLX5BwgxfsiVilMNPbjNEopbsbOgbxqbj297hNRbXpAUKzGXUhZmbHdW7AYH4Pp2kZxBQ5mY2p7OgT%2FmMAEsPEL1ZhNOjSCh4TE%2FDisXexY8z17cwDgjBINpocK%2FXf%2BrQ4iLY5mCMTyvQ34h5x5HJxPF1kXAS%2BgrKXiTXO5T38Ugl0yv8UABtt6wM4ubo2cR0BaCTllzHUrDvL2V4r1mBkaihn1fQK1exY1WSmwlSJR5kaL0IbFvD7JVax8FXxfqrApFZ6AALiuCDe7OiqcseAw3snO%2Fa8eC%2FNUTH1Ox9rmsuoKxsDlMmLZ5DKGd8inqtZAJ9nELBiRLE0iADjLOt5j1I%2BZ5E5Y9EQ08Luqv8vIH5kRsBhrC4OLRUxzMGwFCKCs7GuJYjqnHfEolh6ACZD6RipeCz2MHyGnaHrQMTUg6NhiL7e9%2F8blqgq%2B%2BkOiVoAAAAASUVORK5CYII%3D
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
