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
exports.Interaction = void 0;
var typeorm_1 = require("typeorm");
var locus_entity_1 = require("./locus.entity");
var Interaction = /** @class */ (function () {
    function Interaction() {
    }
    __decorate([
        (0, typeorm_1.PrimaryGeneratedColumn)(),
        __metadata("design:type", Number)
    ], Interaction.prototype, "id", void 0);
    __decorate([
        (0, typeorm_1.OneToOne)(function () { return locus_entity_1.Locus; }),
        (0, typeorm_1.JoinColumn)(),
        __metadata("design:type", locus_entity_1.Locus)
    ], Interaction.prototype, "loc_r", void 0);
    __decorate([
        (0, typeorm_1.OneToOne)(function () { return locus_entity_1.Locus; }),
        (0, typeorm_1.JoinColumn)(),
        __metadata("design:type", locus_entity_1.Locus)
    ], Interaction.prototype, "loc_l", void 0);
    Interaction = __decorate([
        (0, typeorm_1.Entity)()
    ], Interaction);
    return Interaction;
}());
exports.Interaction = Interaction;
//# sourceMappingURL=interaction.entity.js.map