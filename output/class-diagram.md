# Class Diagram

```text
Class Diagram:
  Class: Item
    Attributes: id: int, name: string, quantity: int
    Methods: Add(), Remove()
    Relationships: Inventory

  Class: Inventory
    Attributes: items: List<Item>
    Methods: AddItem(), RemoveItem(), ViewItems()
    Relationships: Item
```

## Детали классов и компонентов

### Класс `Item`
- **Атрибуты:**
  - `id`: `int` (C++: `int`)
  - `name`: `string` (C++: `std::string`)
  - `quantity`: `int` (C++: `int`)
- **Методы:**
  - `Add()` -> `void`
  - `Remove()` -> `void`
- **Связи (Relationships):**
  - Ассоциация с `Inventory`

### Класс `Inventory`
- **Атрибуты:**
  - `items`: `List<Item>` (C++: `std::vector<Item>`)
- **Методы:**
  - `AddItem()` -> `void`
  - `RemoveItem()` -> `void`
  - `ViewItems()` -> `void`
- **Связи (Relationships):**
  - Ассоциация с `Item`

