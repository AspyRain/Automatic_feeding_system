pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
        maven ("https://jitpack.io")// 添加 JitPack 作为镜像库
    }
}

rootProject.name = "AFS_mobile"
include(":app")
include(":app:afs_demo")
include(":app:afs_mobile")
