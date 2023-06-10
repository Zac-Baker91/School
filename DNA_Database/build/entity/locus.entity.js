"use strict";
var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
var __metadata = (this && this.__metadata) || function (k, v) {
    if (typeof Reflect === "object" && typeof Reflect.metadata === "function") return Reflect.metadata(k, v);
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.Locus = void 0;
var typeorm_1 = require("typeorm");
var cell_entity_1 = require("./cell.entity");
var Locus = /** @class */ (function () {
    function Locus() {
    }
    __decorate([
        (0, typeorm_1.PrimaryColumn)(),
        __metadata("design:type", Number)
    ], Locus.prototype, "locus_id", void 0);
    __decorate([
        (0, typeorm_1.Column)(),
        __metadata("design:type", Number)
    ], Locus.prototype, "start", void 0);
    __decorate([
        (0, typeorm_1.Column)(),
        __metadata("design:type", Number)
    ], Locus.prototype, "end", void 0);
    __decorate([
        (0, typeorm_1.Column)(),
        __metadata("design:type", Number)
    ], Locus.prototype, "chrnum", void 0);
    __decorate([
        (0, typeorm_1.ManyToOne)(function () { return cell_entity_1.Cell; }, function (cell) { return cell.type; }),
        __metadata("design:type", cell_entity_1.Cell)
    ], Locus.prototype, "cellType", void 0);
    Locus = __decorate([
        (0, typeorm_1.Entity)()
    ], Locus);
    return Locus;
}());
exports.Locus = Locus;
//# sourceMappingURL=locus.entity.js.map